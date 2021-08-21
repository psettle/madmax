/**
 * Collision instance
 */
#pragma once

#include "engine_Vehicle.hpp"

namespace engine {

class Collision {
 public:
  Collision() : a_(nullptr), b_(nullptr), t_(0) {}
  Collision(Vehicle* a, Vehicle* b, double t) : a_(a), b_(b), t_(t) {}

  double t() const { return t_; }
  Vehicle* a() const { return a_; }
  Vehicle* b() const { return b_; }

 private:
  Vehicle* a_;
  Vehicle* b_;
  double t_;
};

}  // namespace engine
