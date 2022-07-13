#pragma once
#include <fmt/core.h>

#include "types/actor.hpp"
#include "types/world.hpp"

inline auto enemy_turn(World& world) -> void {
  for (auto& actor : world.actors) {
    if (&actor == &world.actors[0]) continue;
    fmt::print("The {} growls!\n", actor.name);
  }
}
