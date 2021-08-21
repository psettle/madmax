/**
 * Current state per player
 */
#pragma once

#include <vector>
#include "engine_Vehicle.hpp"

namespace engine {

class Player {
 public:
  typedef int Id;

  Player(Id id, int score, int rage)
      : id_(id),
        score_(score),
        rage_(rage),
        reaper_(nullptr),
        destroyer_(nullptr),
        doof_(nullptr) {}

  void AddVehicle(Vehicle const& vehicle);

  Id id() const { return id_; }
  int const& rage() const { return rage_; }
  int& rage() { return rage_; }

  Vehicle const& reaper() const { return *reaper_; }
  Vehicle const& destroyer() const { return *destroyer_; }
  Vehicle const& doof() const { return *doof_; }

  Vehicle& reaper() { return *reaper_; }
  Vehicle& destroyer() { return *destroyer_; }
  Vehicle& doof() { return *doof_; }

 private:
  Id const id_;
  int score_;
  int rage_;
  std::vector<Vehicle> vehicles_;
  Vehicle* reaper_;
  Vehicle* destroyer_;
  Vehicle* doof_;
};

}  // namespace engine
