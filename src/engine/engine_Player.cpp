/**
 * Current state per player
 */
#include "engine_Player.hpp"

namespace engine {

void Player::AddVehicle(Vehicle const& vehicle) {
  vehicles_.push_back(vehicle);
  switch (vehicle.type()) {
    case UnitType::kReaper:
      reaper_ = &vehicles_.back();
      break;
    case UnitType::kDestroyer:
      destroyer_ = &vehicles_.back();
      break;
    case UnitType::kDoof:
      doof_ = &vehicles_.back();
      break;
    default:
      break;
  }
}

}  // namespace engine
