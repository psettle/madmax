/**
 * Manipulatable move description
 */
#pragma once

#include <vector>
#include "Vector.hpp"

namespace engine {

class Move {
 public:
  enum class Type { kWait, kSkill, kMove };

  Move() : Move(Type::kWait) {}
  Move(Vector const& target) : Move(Type::kSkill, target) {}
  Move(Vector const& target, int power) : Move(Type::kMove, target, power) {}

  Type type() const { return type_; }
  Vector const& target() const { return target_; }
  int power() const { return power_; }

 private:
  Move(Type type, Vector const& target = Vector(0, 0), int power = 0)
      : type_(type), target_(target), power_(power) {}

  Type type_;
  Vector target_;
  int power_;
};

struct PlayerTurn {
  Move reaper;
  Move destroyer;
  Move doof;
};

struct TotalTurn {
  PlayerTurn player[3];
};

typedef std::vector<TotalTurn> Moves;

}  // namespace engine
