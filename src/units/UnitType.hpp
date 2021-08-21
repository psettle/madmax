/**
 * Possible unit types
 */
#pragma once

enum class UnitType { kReaper, kDestroyer, kDoof, kTanker, kWreck, kTarPool, kOilPool, kUnknown };

UnitType ParseUnitType(int type);
