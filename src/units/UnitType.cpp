/**
 * Possible unit types
 */
#include "UnitType.hpp"

UnitType ParseUnitType(int type) {
  switch (type) {
    case 0:
      return UnitType::kReaper;
    case 1:
      return UnitType::kDestroyer;
    case 2:
      return UnitType::kDoof;
    case 3:
      return UnitType::kTanker;
    case 4:
      return UnitType::kWreck;
    case 5:
      return UnitType::kTarPool;
    case 6:
      return UnitType::kOilPool;
    default:
      return UnitType::kUnknown;
  }
}
