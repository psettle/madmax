/**
 * Description of current gamestate
 */
#include "GameState.hpp"

void GameState::AddPlayer(std::unique_ptr<Player> player) { players_.push_back(std::move(player)); }

void GameState::AddUnit(std::unique_ptr<Vehicle> vehicle, Player::Id owner) {
  for (auto& player : players_) {
    if (player->id() == owner) {
      player->AddVehicle(vehicle.get());
      break;
    }
  }

  if (vehicle->type() == UnitType::kTanker) {
    tankers_.push_back(vehicle.get());
  }
  units_.push_back(std::move(vehicle));
}

void GameState::AddUnit(std::unique_ptr<Unit> unit) {
  if (unit->type() == UnitType::kWreck) {
    wrecks_.push_back(unit.get());
  }
  units_.push_back(std::move(unit));
}
