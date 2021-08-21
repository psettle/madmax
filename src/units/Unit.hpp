/**
 * Base unit properties
 */
#pragma once

#include "UnitType.hpp"
#include "Vector.hpp"

class Unit {
 public:
  typedef int Id;

  virtual ~Unit() {}

  Unit(Id id, UnitType type, double x, double y, double radius)
      : id_(id), type_(type), position_(x, y), radius_(radius){};

  UnitType type() const { return type_; }
  Vector const& position() const { return position_; }
  double radius() const { return radius_; }

 private:
  Id id_;
  UnitType type_;
  Vector position_;
  double radius_;
};
