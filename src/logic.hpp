#pragma once
#include <fmt/core.h>

#include "types/actor.hpp"
#include "types/world.hpp"

/// Create a unique actor and return the mapping iterator to that actor.
inline auto new_actor(World& world) {
  auto uniform = std::uniform_int_distribution<int>();
  while (true) {
    auto new_id = uniform(world.rng);
    auto [iterator, success] = world.actors.insert({new_id, Actor{}});
    if (success) {
      return iterator;
    }
  }
}

inline auto enemy_turn(World& world) -> void {
  for (auto& [actor_id, actor] : world.actors) {
    if (actor_id == 0) continue;
    fmt::print("The {} growls!\n", actor.name);
  }
}
