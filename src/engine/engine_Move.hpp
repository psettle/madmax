/**
 * Manipulatable move description
 */
#pragma once

#include <string>
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

  std::string ToString() const {
    switch (type_) {
      case Type::kWait:
        return "WAIT";
      case Type::kMove:
        return std::to_string(target_.x()) + " " + std::to_string(target_.y()) + " " +
               std::to_string(power_);
      case Type::kSkill:
        return "SKILL " + std::to_string(target_.x()) + " " + std::to_string(target_.y());
    }
    return "";
  }

 private:
  Move(Type type, Vector const& target = Vector(0, 0), int power = 0)
      : type_(type), target_(target), power_(power) {}

  Type type_;
  Vector target_;
  int power_;
};

typedef std::vector<Move> Sequence;

struct PlayerSequence {
  Sequence reaper;
  Sequence destroyer;
  Sequence doof;
};

struct PlayerTurn {
  PlayerTurn(Move const& reaper, Move const& destroyer, Move const& doof)
      : reaper(reaper), destroyer(destroyer), doof(doof) {}

  Move const& reaper;
  Move const& destroyer;
  Move const& doof;
};

struct TotalSequence {
  PlayerSequence player[3];
};

struct TotalTurn {
  TotalTurn(PlayerTurn const& p0, PlayerTurn const& p1, PlayerTurn const& p2)
      : player({p0, p1, p2}) {}

  PlayerTurn player[3];
};

}  // namespace engine
