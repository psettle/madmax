/**
 * Generate move sequences
 */
#pragma once

#include <stdlib.h>
#include "engine_Move.hpp"

namespace monte {

class Generator {
 public:
  Generator(double skill_fraction, double wait_fraction)
      : skill_fraction_(skill_fraction), wait_fraction_(wait_fraction) {}

  void GetSequence(Vector const& origin, int count, engine::Sequence moves_out);

 private:
  engine::Move GetMove(Vector const& origin);

  static double r(double min = -1.0, double max = 1.0) {
    double f = (double)rand() / RAND_MAX;
    return min + f * (max - min);
  }

  double skill_fraction_;
  double wait_fraction_;
};

}  // namespace monte
