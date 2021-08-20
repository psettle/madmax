/**
 * Vehicle unit base class
 */
#pragma once

#include "Unit.hpp"

class Vehicle : public Unit {
 public:
  Vehicle(Id id, UnitType type, double x, double y, double radius, double dx, double dy,
          double mass)
      : Unit(id, type, x, y, radius), velocity_(dx, dy), mass_(mass) {}

 private:
  Vector velocity_;
  double mass_;
};
