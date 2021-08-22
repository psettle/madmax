/**
 * Full simulation of a gamestate
 */
#include "engine_Game.hpp"
#include <algorithm>
#include "engine_Collision.hpp"

namespace engine {

static Vector const kWaterTown = Vector(0.0, 0.0);
static double constexpr kTankerFillRadius = 3000;
static double constexpr kMapRadius = 6000;

Game::Game(State const& initial_state)
    : players_(initial_state.players()),
      tankers_(initial_state.tankers()),
      wrecks_(initial_state.wrecks()),
      skills_(initial_state.skills()) {
  for (auto& player : players_) {
    AddVehicle(&player.reaper());
    AddVehicle(&player.destroyer());
    AddVehicle(&player.doof());
  }

  for (auto& tanker : tankers_) {
    AddVehicle(&tanker);
  }
}

void Game::RunGame(TotalSequence const& all_moves) {
  size_t turn = all_moves.player[0].reaper.size();

  for (size_t i = 0; i < turn; ++i) {
    TotalTurn moves(PlayerTurn(all_moves.player[0].reaper[i], all_moves.player[0].destroyer[i],
                               all_moves.player[0].doof[i]),
                    PlayerTurn(all_moves.player[1].reaper[i], all_moves.player[1].destroyer[i],
                               all_moves.player[1].doof[i]),
                    PlayerTurn(all_moves.player[2].reaper[i], all_moves.player[2].destroyer[i],
                               all_moves.player[2].doof[i]));

    RunTurn(moves);
  }
}

State Game::GetState() const { return State(players_, tankers_, wrecks_, skills_); }

void Game::RunTurn(TotalTurn const& moves) {
  CreateSkills(moves);
  ApplySkills();
  ApplyThrust(moves);
  RunCollisions();
  TankerHarvest();
  RemoveTankers();

  // This is RNG on server side, probably no sense in trying to mimic this
  // Create new tankers

  ApplyOilPools();
  ReaperHarvest();
  RemoveWrecks();
  ApplyFriction();
  RoundUnits();
  GenerateRage();
  RemoveTarPools();
  RemoveOilPools();
  DestroySkills();
}

void Game::CreateSkills(TotalTurn const& moves) {
  static int constexpr kReaperSkillCost = 30;
  static int constexpr kDestroyerSkillCost = 60;
  static int constexpr kDoofSkillCost = 30;
  static int constexpr kSkillRadius = 1000;
  static double constexpr kSkillDistance = 2000;

  static int constexpr kTarDuration = 3;
  static int constexpr kGrenadeDuration = 1;
  static int constexpr kOilDuration = 3;

  for (Player& player : players_) {
    auto const& player_moves = moves.player[player.id()];

    auto const& reaper_move = player_moves.reaper;
    if (reaper_move.type() == Move::Type::kSkill && player.rage() >= kReaperSkillCost &&
        Vector::Distance(reaper_move.target(), player.reaper().position()) <= kSkillDistance) {
      player.rage() -= kReaperSkillCost;
      skills_.push_back(Skill(-1, UnitType::kTarPool, reaper_move.target().x(),
                              reaper_move.target().y(), kSkillRadius, kTarDuration));
    }

    auto const& destroyer_move = player_moves.destroyer;
    if (destroyer_move.type() == Move::Type::kSkill && player.rage() >= kDestroyerSkillCost &&
        Vector::Distance(destroyer_move.target(), player.destroyer().position()) <=
            kSkillDistance) {
      player.rage() -= kDestroyerSkillCost;
      skills_.push_back(Skill(-1, UnitType::kGrenade, destroyer_move.target().x(),
                              destroyer_move.target().y(), kSkillRadius, kGrenadeDuration));
    }

    auto const& doof_move = player_moves.doof;
    if (doof_move.type() == Move::Type::kSkill && player.rage() >= kDoofSkillCost &&
        Vector::Distance(doof_move.target(), player.doof().position()) <= kSkillDistance) {
      player.rage() -= kDoofSkillCost;
      skills_.push_back(Skill(-1, UnitType::kOilPool, doof_move.target().x(),
                              doof_move.target().y(), kSkillRadius, kOilDuration));
    }
  }
}

static int constexpr kTarMass = 10;

void Game::ApplySkills() {
  for (Skill const& skill : skills_) {
    switch (skill.type()) {
      case UnitType::kTarPool:
        ApplyTarPool(skill);
        break;
      case UnitType::kGrenade:
        ApplyGrenadeBoost(skill);
        break;
      default:
        break;
    }
  }
}

void Game::ApplyTarPool(Skill const& pool) {
  for (Vehicle* vehicle : vehicles_) {
    if (Vector::Distance(vehicle->position(), pool.position()) <= pool.radius()) {
      vehicle->AddMass(kTarMass);
    }
  }
}

void Game::ApplyGrenadeBoost(Skill const& grenade) {
  static int constexpr kGrenadePower = 1000;

  for (Vehicle* vehicle : vehicles_) {
    if (Vector::Distance(vehicle->position(), grenade.position()) <= grenade.radius()) {
      vehicle->Thrust(grenade.position(), -kGrenadePower);
    }
  }
}

void Game::ApplyThrust(TotalTurn const& moves) {
  for (Player& player : players_) {
    auto const& player_moves = moves.player[player.id()];

    auto const& reaper_move = player_moves.reaper;
    if (reaper_move.type() == Move::Type::kMove) {
      player.reaper().Thrust(reaper_move.target(), reaper_move.power());
    }

    auto const& destroyer_move = player_moves.destroyer;
    if (destroyer_move.type() == Move::Type::kMove) {
      player.destroyer().Thrust(destroyer_move.target(), destroyer_move.power());
    }

    auto const& doof_move = player_moves.doof;
    if (doof_move.type() == Move::Type::kMove) {
      player.doof().Thrust(doof_move.target(), doof_move.power());
    }
  }

  static int constexpr kTankerThrust = 500.0;
  for (Tanker& tanker : tankers_) {
    if (tanker.full()) {
      // Full, head back
      tanker.Thrust(kWaterTown, -kTankerThrust);
    } else if (Vector::Distance(tanker.position(), kWaterTown) > kTankerFillRadius) {
      // Not full, try to get to water town
      tanker.Thrust(kWaterTown, kTankerThrust);
    }
  }
}

void Game::RunCollisions() {
  double t = 0;
  Collision collision;

  while (GetNextCollision(collision, 1 - t)) {
    RunTime(collision.t());
    RunCollision(collision);
    t += collision.t();
  }

  RunTime(1 - t);
}

void Game::RunTime(double t) {
  for (Vehicle* vehicle : vehicles_) {
    vehicle->RunTime(t);
  }
}

void Game::RunCollision(Collision const& collision) {
  if (collision.a() && collision.b()) {
    if (collision.a()->type() == UnitType::kTanker &&
        collision.b()->type() == UnitType::kDestroyer) {
      WreckTanker(reinterpret_cast<Tanker*>(collision.a()));
    } else if (collision.a()->type() == UnitType::kDestroyer &&
               collision.b()->type() == UnitType::kTanker) {
      WreckTanker(reinterpret_cast<Tanker*>(collision.b()));
    } else {
      Vehicle::Bounce(*collision.a(), *collision.b());
    }
  } else if (collision.a()) {
    Vehicle::Bounce(*collision.a(), kWaterTown, kMapRadius);
  } else {
    Vehicle::Bounce(*collision.b(), kWaterTown, kMapRadius);
  }
}

void Game::WreckTanker(Tanker* tanker) {
  wrecks_.push_back(Wreck(-1, UnitType::kWreck, tanker->position().x(), tanker->position().y(),
                          tanker->radius(), tanker->water()));
  RemoveVehicle(tanker);
  tankers_.erase(std::remove_if(tankers_.begin(), tankers_.end(),
                                [tanker](Tanker const& t) { return &t == tanker; }));
}

bool Game::GetNextCollision(Collision& collision_out, double t_limit) {
  Collision test_collision;
  Collision closest_collision = Collision(nullptr, nullptr, 1000);

  for (auto i = vehicles_.begin(); i != vehicles_.end(); ++i) {
    if (GetNextBorderCollision(test_collision, **i)) {
      if (test_collision.t() <= t_limit && test_collision.t() <= closest_collision.t()) {
        closest_collision = test_collision;
      }
    }

    for (auto j = i + 1; j != vehicles_.end(); ++j) {
      if (GetNextVehicleCollision(test_collision, **i, **j)) {
        if (test_collision.t() <= t_limit && test_collision.t() <= closest_collision.t()) {
          closest_collision = test_collision;
        }
      }
    }
  }

  if (closest_collision.t() <= t_limit) {
    collision_out = closest_collision;
    return true;
  } else {
    return false;
  }
}

bool Game::GetNextBorderCollision(Collision& collision_out, Vehicle& vehicle) {
  if (Vector::Distance(vehicle.position(), kWaterTown) + vehicle.radius() >= kMapRadius) {
    collision_out = Collision(&vehicle, nullptr, 0.0);
    return true;
  }

  // Should probably be <= epsilon but, just this is referee behavior
  if (vehicle.speed2() == 0) {
    return false;
  }

  double a = vehicle.velocity() * vehicle.velocity();
  if (a <= 0) {
    return false;
  }

  double b = 2 * (vehicle.position() * vehicle.velocity());
  double c = vehicle.position() * vehicle.position() -
             (kMapRadius - vehicle.radius()) * (kMapRadius - vehicle.radius());
  double delta = b * b - 4 * a * c;

  if (delta <= 0) {
    return false;
  }

  double t = (-b + std::sqrt(delta)) / (2 * a);
  if (t <= 0) {
    return false;
  }

  collision_out = Collision(&vehicle, nullptr, t);
  return true;
}

bool Game::GetNextVehicleCollision(Collision& collision_out, Vehicle& v1, Vehicle& v2) {
  double r = v1.radius() + v2.radius();

  if (Vector::Distance(v1.position(), v2.position()) <= r) {
    collision_out = Collision(&v1, &v2, 0.0);
    return true;
  }

  if (v2.speed2() == 0.0 && v1.speed2() == 0.0) {
    return false;
  }

  Vector dpos = v1.position() - v2.position();
  Vector dvel = v1.velocity() - v2.velocity();

  double a = dvel * dvel;
  if (a <= 0) {
    return false;
  }

  double b = 2 * (dpos * dvel);
  double c = dpos * dpos - r * r;
  double delta = b * b - 4 * a * c;
  if (delta <= 0.0) {
    return false;
  }

  double t = (-b + std::sqrt(delta)) / (2 * a);
  if (t <= 0) {
    return false;
  }

  collision_out = Collision(&v1, &v2, t);
  return true;
}

void Game::TankerHarvest() {
  for (Tanker& tanker : tankers_) {
    if (!tanker.full() && Vector::Distance(tanker.position(), kWaterTown) <= kTankerFillRadius) {
      tanker.Fill();
    }
  }
}

void Game::RemoveTankers() {
  static double constexpr kTankerKillRadius = 8000;

  auto is_tanker_safe = [&](Tanker& tanker) {
    double distance = Vector::Distance(tanker.position(), kWaterTown) + tanker.radius();
    bool safe = distance >= kTankerKillRadius && tanker.full();

    if (safe) {
      RemoveVehicle(&tanker);
    }

    return safe;
  };

  tankers_.erase(std::remove_if(tankers_.begin(), tankers_.end(), is_tanker_safe));
}

void Game::ApplyOilPools() {
  for (auto const& skill : skills_) {
    if (skill.type() == UnitType::kOilPool) {
      ApplyOilPool(skill);
    }
  }
}

void Game::ApplyOilPool(Skill const& pool) {
  for (Vehicle* vehicle : vehicles_) {
    if (Vector::Distance(vehicle->position(), pool.position()) <= pool.radius()) {
      vehicle->SetOil(true);
    }
  }
}

void Game::ReaperHarvest() {
  for (Player& player : players_) {
    auto& reaper = player.reaper();

    for (Wreck& wreck : wrecks_) {
      if (!reaper.oil() &&
          Vector::Distance(reaper.position(), wreck.position()) <= wreck.radius()) {
        player.AddPoint();
        wreck.Drain();
      }
    }
  }
}

void Game::RemoveWrecks() {
  auto is_wreck_empty = [](Wreck const& wreck) { return wreck.water() <= 0; };

  wrecks_.erase(std::remove_if(wrecks_.begin(), wrecks_.end(), is_wreck_empty));
}

void Game::ApplyFriction() {
  for (Vehicle* vehicle : vehicles_) {
    vehicle->ApplyFriction();
  }
}

void Game::RoundUnits() {
  for (Vehicle* vehicle : vehicles_) {
    vehicle->RoundOff();
  }

  for (Unit& wreck : wrecks_) {
    wreck.RoundOff();
  }
}

void Game::GenerateRage() {
  static int constexpr kRageMax = 300;

  for (auto& player : players_) {
    player.rage() += static_cast<int>(std::floor(player.doof().speed() * 0.01));
    player.rage() = std::min(kRageMax, player.rage());
  }
}

void Game::RemoveTarPools() {
  for (Vehicle* vehicle : vehicles_) {
    vehicle->UnwindMass(kTarMass);
  }
}

void Game::RemoveOilPools() {
  for (Vehicle* vehicle : vehicles_) {
    vehicle->SetOil(false);
  }
}

void Game::DestroySkills() {
  auto is_skill_elapsed = [](Skill const& skill) { return skill.duration() <= 1; };

  skills_.erase(std::remove_if(skills_.begin(), skills_.end(), is_skill_elapsed));
}

void Game::AddVehicle(Vehicle* vehicle) { vehicles_.push_back(vehicle); }

void Game::RemoveVehicle(Vehicle* vehicle) {
  vehicles_.erase(std::find(vehicles_.begin(), vehicles_.end(), vehicle));
}

}  // namespace engine
