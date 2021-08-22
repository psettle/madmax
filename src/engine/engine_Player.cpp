/**
 * Current state per player
 */
#include "engine_Player.hpp"

namespace engine {

void Player::AddVehicle(Vehicle const& vehicle) {
  switch (vehicle.type()) {
    case UnitType::kReaper:
      reaper_ = vehicle;
      break;
    case UnitType::kDestroyer:
      destroyer_ = vehicle;
      break;
    case UnitType::kDoof:
      doof_ = vehicle;
      break;
    default:
      break;
  }
}

}  // namespace engine
