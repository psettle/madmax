/**
 * Full simulation of a gamestate
 */
#pragma once

#include <vector>
#include "engine_Move.hpp"
#include "engine_State.hpp"

namespace engine {

class Game {
 public:
  Game(State const& initial_state);

  void RunGame(Moves const& moves);

 private:
  void RunTurn(TotalTurn const& moves);

  // Setup state for physics
  void CreateSkills(TotalTurn const& moves);
  void ApplySkills();
  void ApplyTarPool(Skill const& pool);
  void ApplyGrenadeBoost(Skill const& grenade);

  // Cleanup state after physics
  void RoundUnits();
  void GenerateRage();
  void RemoveTarPools();
  void DestroySkills();

  std::vector<Player> players_;
  std::vector<Tanker> tankers_;
  std::vector<Unit> wrecks_;
  std::vector<Skill> skills_;
  std::vector<Vehicle*> vehicles_;
};

}  // namespace engine
