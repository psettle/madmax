/**
 * Vehicle unit base class
 */
#pragma once

#include "engine_Unit.hpp"

namespace engine {

class Vehicle : public Unit {
 public:
  Vehicle(Id id, UnitType type, double x, double y, double radius, double dx, double dy,
          double mass, double friction)
      : Unit(id, type, x, y, radius),
        velocity_(dx, dy),
        mass_(mass),
        oil_(false),
        friction_(friction) {}

  double speed() const { return Vector::Distance(Vector(0, 0), position()); }
  bool oil() const { return oil_; }

  void AddMass(double mass) { mass_ += mass; }
  void UnwindMass(double mass) {
    while (mass_ > mass) {
      mass_ -= mass;
    }
  }

  void Thrust(Vector const& direction, double power);

  void RoundOff() override {
    Unit::RoundOff();
    velocity_.Round();
  }

  void SetOil(bool status) { oil_ = status; }

  void ApplyFriction() {
    if (oil()) {
      return;
    }

    velocity_ *= (1.0 - friction_);
  }

  virtual double mass() const { return mass_; }

 private:
  Vector velocity_;
  double mass_;
  bool oil_;
  double friction_;
};

}  // namespace engine
