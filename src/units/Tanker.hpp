/**
 * Tanker class
 */
#pragma once

#include "Vehicle.hpp"

class Tanker : public Vehicle {
 public:
  Tanker(Id id, UnitType type, double x, double y, double radius, double dx, double dy, double mass,
         int water, int capacity)
      : Vehicle(id, type, x, y, radius, dx, dy, mass), water_(water), capacity_(capacity) {}

 private:
  int water_;
  int capacity_;
};
