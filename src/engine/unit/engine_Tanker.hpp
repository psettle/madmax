/**
 * Tanker class
 */
#pragma once

#include "engine_Vehicle.hpp"

namespace engine {

class Tanker : public Vehicle {
 public:
  Tanker(Id id, UnitType type, double x, double y, double radius, double dx, double dy, int water,
         int capacity)
      : Vehicle(id, type, x, y, radius, dx, dy, 2.5, 0.4), water_(water), capacity_(capacity) {}

  double mass() const override { return Vehicle::mass() + water() / 2.0; }
  bool full() const { return water() == capacity_; }
  int water() const { return water_; }

  void Fill() { water_++; }

 private:
  int water_;
  int capacity_;
};

}  // namespace engine
