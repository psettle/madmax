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
        base_mass_(mass),
        oil_(false),
        friction_(friction) {}

  Vehicle() : Unit(-1, UnitType::kUnknown, 0, 0, 0), velocity_(0, 0) {}

  double speed() const { return std::sqrt(speed2()); }
  double speed2() const { return velocity() * velocity(); }
  bool oil() const { return oil_; }
  Vector const& velocity() const { return velocity_; }

  void AddMass(double mass) { base_mass_ += mass; }
  void UnwindMass(double mass) {
    while (base_mass_ > mass) {
      base_mass_ -= mass;
    }
  }

  void Thrust(Vector const& target, double power);
  void RunTime(double t);
  static void Bounce(Vehicle& a, Vehicle& b);
  static void Bounce(Vehicle& v, Vector const& centre, double radius);

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

  virtual double mass() const { return base_mass_; }

 private:
  Vector velocity_;
  double base_mass_;
  bool oil_;
  double friction_;
};

}  // namespace engine
