/**
 * Full simulation of a gamestate
 */
#include "engine_Game.hpp"

namespace engine {
Game::Game(GameState const& initial_state) : state_(initial_state) {}

void Game::RunGame(std::vector<MoveSequence> const& moves_per_player) {
  // Create skill effects

  // Apply tar pools
  // Apply grenade
  // Vehicles accelerate
  // Movement + collisions
  // Remove full tankers
  // Create new tankers
  // Reapers harvest water
  // Remove empty wrecks
  // Apply friction
  // Round positions/velocity
  // Generate rage
  // Unapply tar pools
  // Remove tar pools
}
}  // namespace engine
