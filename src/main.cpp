#include "cg-pragma.hpp"
// optimize pragmas first

#include <iostream>
#include "Reaper.hpp"
#include "engine_Game.hpp"
#include "engine_State.hpp"

int main() {
  // game loop
  while (1) {
    engine::State state;

    std::cout << get_reaper_move(state) << std::endl;
    std::cout << "WAIT" << std::endl;
    std::cout << "WAIT" << std::endl;
  }
}
