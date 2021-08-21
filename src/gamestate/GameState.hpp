/**
 * Description of current gamestate
 */
#pragma once

#include <vector>
#include "Player.hpp"
#include "Skill.hpp"
#include "Tanker.hpp"
#include "Unit.hpp"
#include "Vehicle.hpp"

class GameState {
 public:
  GameState();  // read gamestate from stdin

  std::vector<Unit> const& wrecks() const { return wrecks_; }

  Player const& GetPlayer(Player::Id id) const { return players_[id]; }

 private:
  void AddVehicle(Vehicle const& vehicle, Player::Id owner);
  void AddWreck(Unit const& unit);
  void AddSkill(Skill const& skill);
  void AddTanker(Tanker const& tanker);

  std::vector<Player> players_;
  std::vector<Tanker> tankers_;
  std::vector<Unit> wrecks_;
  std::vector<Skill> skills_;
};
