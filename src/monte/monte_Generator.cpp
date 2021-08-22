/**
 * Generate move sequences
 */
#include "monte_Generator.hpp"

namespace monte {

static double constexpr kPi = 3.14159265359;

void Generator::GetSequence(Vector const& origin, int count, engine::Sequence moves_out) {
  while (count-- > 0) {
    moves_out.push_back(GetMove(origin));
  }
}

engine::Move Generator::GetMove(Vector const& origin) {
  double type_random = r(0.0, 1.0);
  if (type_random <= skill_fraction_) {
    // Skill move
    double distance = r(0.0, 1.0) * 6000;
    double angle = r(0.0, 1.0) * 2 * kPi;

    Vector target(std::cos(angle), std::sin(angle));
    target *= distance;

    return engine::Move(target);
  } else if (type_random <= (skill_fraction_ + wait_fraction_)) {
    // Wait move
    return engine::Move();
  } else {
    // Thrust move
    double distance = r(0.0, 1.0) * 6000;
    double angle = r(0.0, 1.0) * 2 * kPi;

    Vector direction(std::cos(angle), std::sin(angle));
    Vector target = origin + direction * distance;
    return engine::Move(target, r(0.0, 1.0) * 300);
  }
}

}  // namespace monte
