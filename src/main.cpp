#include "cg-pragma.hpp"
// optimize pragmas first

#include <cmath>
#include <iostream>
#include <vector>

struct Player {
  int x;
  int y;
  int vx;
  int vy;
};

struct Water {
  int x;
  int y;
  int radius;
  int water;
};

int main() {
  // game loop
  while (1) {
    Player p;
    std::vector<Water> water;

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

      if (unitType == 4) {
        water.push_back(Water{x, y, radius, extra});
      }

      if (player == 0 && unitType == 0) {
        p.x = x;
        p.y = y;
        p.vx = vx;
        p.vy = vy;
      }
    }

    Water const* closest = nullptr;
    int closest_dist = 0xFFFFFFF;
    for (auto const& w : water) {
      int dist = (p.x - w.x) * (p.x - w.x) + (p.y - w.y) * (p.y - w.y);
      if (dist < closest_dist) {
        closest_dist = dist;
        closest = &w;
      }
    }

    if (closest) {
      closest_dist = std::max(300, static_cast<int>(std::sqrt(closest_dist) - closest->radius));
      std::cout << closest->x << " " << closest->y << " " << closest_dist << std::endl;
    } else {
      std::cout << "WAIT" << std::endl;
    }

    std::cout << "WAIT" << std::endl;
    std::cout << "WAIT" << std::endl;
  }
}
