/**
 * Full simulation of a gamestate
 */
#pragma once

#include <vector>
#include "GameState.hpp"
#include "engine_Move.hpp"

namespace engine {

class Game {
 public:
  Game(GameState const& initial_state);

  void RunGame(std::vector<MoveSequence> const& moves_per_player);

 private:
  GameState state_;
};

}  // namespace engine
