#include "cg-pragma.hpp"
// optimize pragmas first

#include <iostream>
#include "GameState.hpp"
#include "Reaper.hpp"

int main() {
  // game loop
  while (1) {
    GameState state;

    int myScore;
    std::cin >> myScore;
    int enemyScore1;
    std::cin >> enemyScore1;
    int enemyScore2;
    std::cin >> enemyScore2;
    int myRage;
    std::cin >> myRage;
    int enemyRage1;
    std::cin >> enemyRage1;
    int enemyRage2;
    std::cin >> enemyRage2;

    state.AddPlayer(std::make_unique<Player>(0, myScore, myRage));
    state.AddPlayer(std::make_unique<Player>(1, enemyScore1, enemyRage1));
    state.AddPlayer(std::make_unique<Player>(2, enemyScore2, enemyRage2));

    int unitCount;
    std::cin >> unitCount;
    for (int i = 0; i < unitCount; i++) {
      int unitId;
      int unitType;
      int player;
      float mass;
      int radius;
      int x;
      int y;
      int vx;
      int vy;
      int extra;
      int extra2;
      std::cin >> unitId >> unitType >> player >> mass >> radius >> x >> y >> vx >> vy >> extra >>
          extra2;

      UnitType type = parse_unit_type(unitType);

      if (type == UnitType::kWreck) {
        state.AddUnit(std::make_unique<Unit>(unitId, type, x, y, radius));
      } else {
        state.AddUnit(std::make_unique<Vehicle>(unitId, type, x, y, radius, vx, vy, mass), player);
      }
    }

    std::cout << get_reaper_move(state) << std::endl;
    std::cout << "WAIT " << state.GetPlayer(0).reaper().position().x() << std::endl;
    std::cout << "WAIT" << std::endl;
  }
}
