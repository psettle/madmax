/**
 * Description of current gamestate
 */
#include "engine_State.hpp"
#include <iostream>

namespace engine {

void State::AddVehicle(Vehicle const& vehicle, Player::Id owner) {
  players_[owner].AddVehicle(vehicle);
}

void State::AddWreck(Wreck const& unit) { wrecks_.push_back(unit); }

void State::AddSkill(Skill const& skill) { skills_.push_back(skill); }

void State::AddTanker(Tanker const& tanker) { tankers_.push_back(tanker); }

State::State() {
  int score_0, score_1, score_2, rage_0, rage_1, rage_2;
  std::cin >> score_0 >> score_1 >> score_2 >> rage_0 >> rage_1 >> rage_2;

  players_.push_back(Player(0, score_0, rage_0));
  players_.push_back(Player(1, score_1, rage_1));
  players_.push_back(Player(2, score_2, rage_2));

  int unitCount;
  std::cin >> unitCount;
  for (int i = 0; i < unitCount; i++) {
    int unitId, unitType, player;
    float mass;
    int radius;
    int x, y;
    int vx, vy;
    int extra, extra2;
    std::cin >> unitId >> unitType >> player >> mass >> radius >> x >> y >> vx >> vy >> extra >>
        extra2;

    UnitType type = ParseUnitType(unitType);

    switch (type) {
      case UnitType::kWreck:
        AddWreck(Wreck(unitId, type, x, y, radius, extra));
        break;
      case UnitType::kOilPool:
      case UnitType::kTarPool:
        AddSkill(Skill(unitId, type, x, y, radius, extra));
        break;
      case UnitType::kTanker:
        AddTanker(Tanker(unitId, type, x, y, radius, vx, vy, extra, extra2));
        break;
      case UnitType::kReaper:
        AddVehicle(Vehicle(unitId, type, x, y, radius, vx, vy, 0.5, 0.2), player);
        break;
      case UnitType::kDestroyer:
        AddVehicle(Vehicle(unitId, type, x, y, radius, vx, vy, 1.5, 0.3), player);
        break;
      case UnitType::kDoof:
        AddVehicle(Vehicle(unitId, type, x, y, radius, vx, vy, 1.0, 0.25), player);
        break;
      default:
        break;
    }
  }
}

}  // namespace engine
