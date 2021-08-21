/**
 * Full simulation of a gamestate
 */
#include "engine_Game.hpp"
#include <algorithm>

namespace engine {

static Vector const kWaterTown = Vector(0.0, 0.0);
static double constexpr kTankerFillRadius = 3000;

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

void Game::RunGame(Moves const& moves) {
  for (auto const& move : moves) {
    RunTurn(move);
  }
}

void Game::RunTurn(TotalTurn const& moves) {
  CreateSkills(moves);
  ApplySkills();
  ApplyThrust(moves);

  // Movement + collisions

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
