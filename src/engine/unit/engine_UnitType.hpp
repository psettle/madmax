/**
 * Possible unit types
 */
#pragma once

namespace engine {

enum class UnitType {
  kReaper,
  kDestroyer,
  kDoof,
  kTanker,
  kWreck,
  kTarPool,
  kOilPool,
  kGrenade,
  kUnknown
};

UnitType ParseUnitType(int type);

}  // namespace engine
