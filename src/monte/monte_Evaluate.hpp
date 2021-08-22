/**
 * Evaluate move sequences
 */
#pragma once

#include "engine_State.hpp"
#include "monte_Generator.hpp"

namespace monte {

double Evaluate(engine::State const& initial_state, engine::TotalSequence const& moves,
                engine::Player::Id for_player);

}  // namespace monte
