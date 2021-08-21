/**
 * Skill unit base class
 */
#pragma once

#include "engine_Unit.hpp"

namespace engine {

class Skill : public Unit {
 public:
  Skill(Id id, UnitType type, double x, double y, double radius, int duration)
      : Unit(id, type, x, y, radius), duration_(duration) {}

  int duration() const { return duration_; }

 private:
  int duration_;
};

}  // namespace engine
