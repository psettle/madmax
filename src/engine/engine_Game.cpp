/**
 * Full simulation of a gamestate
 */
#include "engine_Game.hpp"

namespace engine {

Game::Game(State const& initial_state)
    : players_(initial_state.players()),
      tankers_(initial_state.tankers()),
      wrecks_(initial_state.wrecks()),
      skills_(initial_state.skills()) {
  for (auto& player : players_) {
    vehicles_.push_back(&player.reaper());
    vehicles_.push_back(&player.destroyer());
    vehicles_.push_back(&player.doof());
  }

  for (auto& tanker : tankers_) {
    vehicles_.push_back(&tanker);
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

  // Vehicles accelerate
  // Movement + collisions
  // tankers harvest water
  // Remove full tankers
  // Create new tankers
  // Reapers harvest water
  // Remove empty wrecks
  // Apply friction

  RoundUnits();
  GenerateRage();
  RemoveTarPools();
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
    if (reaper_move.type == Move::Type::kSkill && player.rage() >= kReaperSkillCost &&
        Vector::Distance(reaper_move.target, player.reaper().position()) <= kSkillDistance) {
      player.rage() -= kReaperSkillCost;
      skills_.push_back(Skill(-1, UnitType::kTarPool, reaper_move.target.x(),
                              reaper_move.target.y(), kSkillRadius, kTarDuration));
    }

    auto const& destroyer_move = player_moves.destroyer;
    if (destroyer_move.type == Move::Type::kSkill && player.rage() >= kDestroyerSkillCost &&
        Vector::Distance(destroyer_move.target, player.destroyer().position()) <= kSkillDistance) {
      player.rage() -= kDestroyerSkillCost;
      skills_.push_back(Skill(-1, UnitType::kGrenade, destroyer_move.target.x(),
                              destroyer_move.target.y(), kSkillRadius, kGrenadeDuration));
    }

    auto const& doof_move = player_moves.doof;
    if (doof_move.type == Move::Type::kSkill && player.rage() >= kDoofSkillCost &&
        Vector::Distance(doof_move.target, player.doof().position()) <= kSkillDistance) {
      player.rage() -= kDoofSkillCost;
      skills_.push_back(Skill(-1, UnitType::kOilPool, doof_move.target.x(), doof_move.target.y(),
                              kSkillRadius, kOilDuration));
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
  for (Skill const& skill : skills_) {
    if (skill.type() == UnitType::kTarPool) {
      for (Vehicle* vehicle : vehicles_) {
        vehicle->UnwindMass(kTarMass);
      }
    }
  }
}

void Game::DestroySkills() {
  // int iterator to allow -1 -> 0 transition
  for (int i = 0; i < static_cast<int>(skills_.size()); ++i) {
    if (skills_[i].duration() <= 1) {
      skills_.erase(skills_.begin() + i);
      // rewind to check whatever is pushing into erased slot
      // could use tree/hash but probably don't have enough skills to warrant it
      i--;
    }
  }
}

}  // namespace engine
