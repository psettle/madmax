/**
 * Description of current gamestate
 */
#pragma once

#include <memory>
#include <vector>
#include "Player.hpp"
#include "Unit.hpp"
#include "Vehicle.hpp"

class GameState {
 public:
  void AddPlayer(std::unique_ptr<Player> player);
  void AddUnit(std::unique_ptr<Vehicle> vehicle, Player::Id owner);
  void AddUnit(std::unique_ptr<Unit> unit);

  std::vector<Unit const*> const& wrecks() const { return wrecks_; }

  Player const& GetPlayer(Player::Id id) const { return *players_[id]; }

 private:
  std::vector<std::unique_ptr<Unit const>> units_;
  std::vector<std::unique_ptr<Player>> players_;
  std::vector<Vehicle const*> tankers_;
  std::vector<Unit const*> wrecks_;
};
