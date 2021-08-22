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
#include "engine_Wreck.hpp"

namespace engine {

class State {
 public:
  State();  // read gamestate from stdin

  State(std::vector<Player> const& players, std::vector<Tanker> const& tankers,
        std::vector<Wreck> const& wrecks, std::vector<Skill> const& skills)
      : players_(players), tankers_(tankers), wrecks_(wrecks), skills_(skills) {}

  std::vector<Wreck> const& wrecks() const { return wrecks_; }
  std::vector<Player> const& players() const { return players_; }
  std::vector<Tanker> const& tankers() const { return tankers_; }
  std::vector<Skill> const& skills() const { return skills_; }

 private:
  void AddWreck(Wreck const& unit);
  void AddSkill(Skill const& skill);
  void AddVehicle(Vehicle const& vehicle, Player::Id owner);
  void AddTanker(Tanker const& tanker);

  std::vector<Player> players_;
  std::vector<Tanker> tankers_;
  std::vector<Wreck> wrecks_;
  std::vector<Skill> skills_;
};

}  // namespace engine
