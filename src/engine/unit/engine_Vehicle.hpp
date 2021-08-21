/**
 * Vehicle unit base class
 */
#pragma once

#include "engine_Unit.hpp"

namespace engine {

class Vehicle : public Unit {
 public:
  Vehicle(Id id, UnitType type, double x, double y, double radius, double dx, double dy,
          double mass)
      : Unit(id, type, x, y, radius), velocity_(dx, dy), mass_(mass) {}

  void AddMass(double mass) { mass_ += mass; }
  void UnwindMass(double mass) {
    while (mass_ > mass) {
      mass_ -= mass;
    }
  }

  void Thrust(Vector const& direction, double power);

  double speed() const { return Vector::Distance(Vector(0, 0), position()); }

  void RoundOff() override {
    Unit::RoundOff();
    velocity_.Round();
  }

 private:
  Vector velocity_;
  double mass_;
};

}  // namespace engine
