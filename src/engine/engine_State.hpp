/**
 * Description of current gamestate
 */
#pragma once

#include <vector>
#include "engine_Player.hpp"
#include "engine_Skill.hpp"
#include "engine_Tanker.hpp"
#include "engine_Unit.hpp"
#include "engine_Vehicle.hpp"

namespace engine {

class State {
 public:
  State();  // read gamestate from stdin

  std::vector<Unit> const& wrecks() const { return wrecks_; }
  std::vector<Player> const& players() const { return players_; }
  std::vector<Tanker> const& tankers() const { return tankers_; }
  std::vector<Skill> const& skills() const { return skills_; }

  void AddWreck(Unit const& unit);
  void AddSkill(Skill const& skill);

 private:
  void AddVehicle(Vehicle const& vehicle, Player::Id owner);
  void AddTanker(Tanker const& tanker);

  std::vector<Player> players_;
  std::vector<Tanker> tankers_;
  std::vector<Unit> wrecks_;
  std::vector<Skill> skills_;
};

}  // namespace engine
