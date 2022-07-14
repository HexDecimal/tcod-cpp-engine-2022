#pragma once
#include <fmt/core.h>

#include "types/actor.hpp"
#include "types/world.hpp"

/// Create a unique actor and return the mapping iterator to that actor.
inline auto new_actor(World& world) {
  auto uniform = std::uniform_int_distribution<ActorID>();
  while (true) {
    auto new_id = uniform(world.rng);
    auto [iterator, success] = world.actors.insert({new_id, Actor{}});
    if (success) {
      return iterator;
    }
  }
}

inline auto enemy_turn(World& world) -> void {
  assert(world.schedule.front() == 0);
  world.schedule.push_back(world.schedule.front());
  world.schedule.pop_front();
  while (world.schedule.front() != 0) {
    auto actor_id = world.schedule.front();
    world.schedule.push_back(actor_id);
    world.schedule.pop_front();
    auto& actor = world.actors.at(actor_id);
    world.log.append(fmt::format("The {} growls!", actor.name));
    fmt::print("The {} ({:08X}) growls!\n", actor.name, actor_id);
  }
}
