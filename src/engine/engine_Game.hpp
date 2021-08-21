/**
 * Full simulation of a gamestate
 */
#pragma once

#include <vector>
#include "engine_Collision.hpp"
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
  void ApplyThrust(TotalTurn const& moves);

  // physics
  void RunCollisions();
  void RunTime(double t);
  void RunCollision(Collision const& collision);
  void WreckTanker(Tanker* tanker);
  bool GetNextCollision(Collision& collision_out, double t_limit);
  bool GetNextBorderCollision(Collision& collision_out, Vehicle& vehicle);
  bool GetNextVehicleCollision(Collision& collision_out, Vehicle& a, Vehicle& b);

  // Cleanup state after physics
  void TankerHarvest();
  void RemoveTankers();
  void ApplyOilPools();
  void ApplyOilPool(Skill const& pool);
  void ReaperHarvest();
  void RemoveWrecks();
  void ApplyFriction();
  void RoundUnits();
  void GenerateRage();
  void RemoveTarPools();
  void RemoveOilPools();
  void DestroySkills();

  // utility
  void AddVehicle(Vehicle* vehicle);
  void RemoveVehicle(Vehicle* vehicle);

  std::vector<Player> players_;
  std::vector<Tanker> tankers_;
  std::vector<Wreck> wrecks_;
  std::vector<Skill> skills_;
  std::vector<Vehicle*> vehicles_;
};

}  // namespace engine
