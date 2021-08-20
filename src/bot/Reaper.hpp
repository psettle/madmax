/**
 * Reaper logic
 */
#pragma once
#include <math.h>
#include <iostream>
#include <string>
#include "GameState.hpp"

std::string get_reaper_move(GameState const& state) {
  auto const& reaper = state.GetPlayer(0).reaper();

  double closest_wreck_distance = INFINITY;
  Unit const* closest_wreck = nullptr;

  for (auto const& wreck : state.wrecks()) {
    double distance = Vector::Distance(reaper.position(), wreck->position());

    if (distance < closest_wreck_distance) {
      closest_wreck_distance = distance;
      closest_wreck = wreck;
    }
  }

  if (!closest_wreck) {
    return "WAIT";
  }

  if (closest_wreck_distance < closest_wreck->radius()) {
    /* Actively collecting, wait until wreck dries up. */
    return "WAIT";
  }

  double thrust = std::min(300.0, (closest_wreck_distance - closest_wreck->radius()) / 2.0);
  std::cerr << thrust << std::endl;
  return std::to_string((int)closest_wreck->position().x()) + " " +
         std::to_string((int)closest_wreck->position().y()) + " " + std::to_string((int)thrust);
}
