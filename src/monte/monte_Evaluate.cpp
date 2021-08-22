/**
 * Evaluate moves
 */
#include "monte_Evaluate.hpp"
#include "engine_Game.hpp"

namespace monte {

static double EvaluateGameState(engine::State const& final_state, engine::Player::Id for_player);

double Evaluate(engine::State const& initial_state, engine::TotalSequence const& moves,
                engine::Player::Id for_player) {
  engine::Game game(initial_state);
  game.RunGame(moves);

  return EvaluateGameState(game.GetState(), for_player);
}

static double EvaluateGameState(engine::State const& final_state, engine::Player::Id for_player) {
  double score = 0;

  for (int i = 0; i < 3; ++i) {
    if (i == for_player) {
      score += final_state.players()[i].score();
    } else {
      score -= final_state.players()[i].score();
    }
  }

  return score;
}

}  // namespace monte
