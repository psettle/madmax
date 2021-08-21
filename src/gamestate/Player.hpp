/**
 * Current state per player
 */
#pragma once

#include <vector>
#include "Vehicle.hpp"

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

  Vehicle const& reaper() const { return *reaper_; }
  Vehicle const& destroyer() const { return *destroyer_; }
  Vehicle const& doof() const { return *doof_; }

 private:
  Id const id_;
  int const score_;
  int const rage_;
  std::vector<Vehicle> vehicles_;
  Vehicle* reaper_;
  Vehicle* destroyer_;
  Vehicle* doof_;
};
