/**
 * Wreck class
 */
#pragma once

#include "engine_Unit.hpp"

namespace engine {

class Wreck : public Unit {
 public:
  Wreck(Id id, UnitType type, double x, double y, double radius, int water)
      : Unit(id, type, x, y, radius), water_(water) {}

  int water() const { return water_; }

  void Drain() { water_--; }

 private:
  int water_;
};

}  // namespace engine
