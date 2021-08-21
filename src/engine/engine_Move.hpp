/**
 * Manipulatable move description
 */
#pragma once

#include <vector>
#include "Vector.hpp"

namespace engine {
struct Move {
  enum class Type { kWait, kSkill, kMove };

  Move(Type t) : Move(t, 0, 0) {}
  Move(Type t, double x, double y) : Move(t, Vector(x, y)) {}
  Move(Type t, Vector const& p) : type(t), target(p) {}

  Type type;
  Vector target;
};

typedef std::vector<Move> MoveSequence;
}  // namespace engine