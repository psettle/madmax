#include "cg-pragma.hpp"
// optimize pragmas first

#include <iostream>
#include "GameState.hpp"
#include "Reaper.hpp"

int main() {
  // game loop
  while (1) {
    GameState state;

    std::cout << get_reaper_move(state) << std::endl;
    std::cout << "WAIT " << state.GetPlayer(0).reaper().position().x() << std::endl;
    std::cout << "WAIT" << std::endl;
  }
}
