/**
 * Skill unit base class
 */
#pragma once

#include "Unit.hpp"

class Skill : public Unit {
 public:
  Skill(Id id, UnitType type, double x, double y, double radius, int duration)
      : Unit(id, type, x, y, radius), duration_(duration) {}

 private:
  int duration_;
};
