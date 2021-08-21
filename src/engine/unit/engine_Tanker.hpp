/**
 * Tanker class
 */
#pragma once

#include "engine_Vehicle.hpp"

namespace engine {

class Tanker : public Vehicle {
 public:
  Tanker(Id id, UnitType type, double x, double y, double radius, double dx, double dy, double mass,
         int water, int capacity)
      : Vehicle(id, type, x, y, radius, dx, dy, mass), water_(water), capacity_(capacity) {}

 private:
  int water_;
  int capacity_;
};

}  // namespace engine
