/**
 * Possible unit types
 */
#pragma once

enum class UnitType { kReaper, kDestroyer, kDoof, kTanker, kWreck, kUnknown };

UnitType parse_unit_type(int type);
