#include "cg-pragma.hpp"
// optimize pragmas first

#include <iostream>
#include "monte_Optimize.hpp"

int main() {
  // game loop
  while (1) {
    engine::Move reaper, destroyer, doof;

    monte::Optimize(reaper, destroyer, doof);

    std::cout << reaper.ToString() << std::endl;
    std::cout << destroyer.ToString() << std::endl;
    std::cout << doof.ToString() << std::endl;
  }
}
