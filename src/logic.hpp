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
      iterator->second.id = new_id;
      return iterator;
    }
  }
}

inline auto enemy_turn(World& world) -> void {
  assert(world.schedule.front() == 0);
  world.schedule.push_back(world.schedule.front());
  world.schedule.pop_front();
  while (world.schedule.front() != 0 && world.actors.find(0) != world.actors.end()) {
    auto actor_id = world.schedule.front();
    world.schedule.pop_front();
    auto actor_it = world.actors.find(actor_id);
    if (actor_it == world.actors.end()) {
      fmt::print("Dropped missing actor {:0X} from schedule.\n", actor_id);
      continue;
    }
    auto& actor = actor_it->second;
    if (actor.ai) actor.ai->perform(world, actor);
    world.schedule.push_back(actor_id);
  }
}
