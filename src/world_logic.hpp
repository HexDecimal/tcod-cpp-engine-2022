#pragma once
#include <fmt/core.h>

#include "types/actor.hpp"
#include "types/world.hpp"

/// Create a unique actor and return the mapping iterator to that actor.
inline auto new_actor(World& world) {
  while (true) {
    auto new_id = ActorID{world.rng()};
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
    if (actor.ai) {
      const auto result = actor.ai->perform(world, actor);
      if (std::holds_alternative<action::Failure>(result)) {
        fmt::print("AI failed action: {}\n", std::get<action::Failure>(result).reason);
      } else if (std::holds_alternative<action::Success>(result)) {
      } else {
        assert(0);
      }
    }
    world.schedule.push_back(actor_id);
  }
}
/// Return a pointer to the actor closest to `pos`.
template <typename DistType = int, typename DistFunc, typename ValidActorFunc>
inline auto get_closest_actor(
    World& world,
    Position pos,
    const DistFunc distance_function,
    const ValidActorFunc is_valid_actor = [](Actor&) -> bool { return true; },
    DistType max_distance = std::numeric_limits<DistType>::max()) {
  Actor* best_actor = nullptr;
  DistType best_distance = max_distance;
  for (auto& [actor_id, actor] : world.actors) {
    if (!is_valid_actor(actor)) continue;
    const DistType new_distance = distance_function(actor.pos - pos);
    if (new_distance < best_distance) {
      best_distance = new_distance;
      best_actor = &actor;
    }
  }
  return best_actor;
}
