/**
 * Base unit properties
 */
#pragma once

#include "Vector.hpp"
#include "engine_UnitType.hpp"

namespace engine {

class Unit {
 public:
  typedef int Id;

  virtual ~Unit() {}

  Unit(Id id, UnitType type, double x, double y, double radius)
      : id_(id), type_(type), position_(x, y), radius_(radius){};

  UnitType type() const { return type_; }
  Vector const& position() const { return position_; }
  double radius() const { return radius_; }

  virtual void RoundOff() { position_.Round(); }

  void Move(Vector const& delta) { position_ += delta; }

  void Move(Vector const& target, double distance) {
    double d = Vector::Distance(position(), target);

    if (d < kEpsilon) {
      return;
    }

    Vector delta = target - position();
    double coef = distance / d;
    Move(delta * coef);
  }

 private:
  Id id_;
  UnitType type_;
  Vector position_;
  double radius_;
};

}  // namespace engine
