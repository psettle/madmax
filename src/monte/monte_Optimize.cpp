/**
 * Generate and evaluate paths to find a good one
 */
#include "monte_Optimize.hpp"
#include "engine_State.hpp"
#include "monte_Evaluate.hpp"
#include "monte_Generator.hpp"

namespace monte {

static int constexpr kDepth = 5;

void Optimize(engine::Move reaper_out, engine::Move destroyer_out, engine::Move doof_out) {
  engine::State current;
  Generator gen(0.1, 0.1);

  // Initialize some random moves to optimize within
  engine::TotalSequence test_moves;
  for (int i = 0; i < 3; ++i) {
    gen.GetSequence(current.players()[i].reaper().position(), kDepth, test_moves.player[i].reaper);
    gen.GetSequence(current.players()[i].destroyer().position(), kDepth,
                    test_moves.player[i].destroyer);
    gen.GetSequence(current.players()[i].doof().position(), kDepth, test_moves.player[i].doof);
  }

  // Optimize our reaper
  engine::TotalSequence best_moves = test_moves;
  double best_score = monte::Evaluate(current, best_moves, 0);

  for (int i = 0; i < 100; ++i) {
    gen.GetSequence(current.players()[0].reaper().position(), kDepth, test_moves.player[i].reaper);
    double score = monte::Evaluate(current, test_moves, 0);

    if (score > best_score) {
      best_score = score;
      best_moves = test_moves;
    }
  }

  reaper_out = best_moves.player[0].reaper.front();
  destroyer_out = best_moves.player[0].destroyer.front();
  doof_out = best_moves.player[0].doof.front();
}

}  // namespace monte
