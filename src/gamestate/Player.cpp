/**
 * Current state per player
 */
#include "Player.hpp"

void Player::AddVehicle(Vehicle const* vehicle) {
  switch (vehicle->type()) {
    case UnitType::kReaper:
      reaper_ = vehicle;
      break;
    case UnitType::kDestroyer:
      destroyer_ = vehicle;
      break;
    case UnitType::kDoof:
      doof_ = vehicle;
      break;

      break;
    default:
      break;
  }
}
