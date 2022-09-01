#pragma once
#include <fmt/core.h>

#include <gsl/gsl>

#include "distance.hpp"
#include "types/actor.hpp"
#include "types/world.hpp"

/// Create a unique actor and return the mapping iterator to that actor.
inline auto new_actor(World& world) {
  while (true) {
    // RNG's need to be narrowed on some implementations.
    auto new_id = ActorID{gsl::narrow<std::underlying_type_t<ActorID>>(world.rng())};
    auto [iterator, success] = world.actors.insert({new_id, Actor{}});
    if (success) {
      iterator->second.id = new_id;
      return iterator;
    }
  }
}

inline auto enemy_turn(World& world) -> void {
  assert(world.schedule.front() == ActorID{0});
  world.schedule.push_back(world.schedule.front());
  world.schedule.pop_front();
  while (world.schedule.front() != ActorID{0} && world.actors.find(ActorID{0}) != world.actors.end()) {
    auto actor_id = world.schedule.front();
    world.schedule.pop_front();
    auto actor_it = world.actors.find(actor_id);
    if (actor_it == world.actors.end()) {
      fmt::print(
          "Dropped missing actor {:0X} from schedule.\n", static_cast<std::underlying_type_t<ActorID>>(actor_id));
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
inline auto get_nearest_actor(
    World& world,
    Position pos,
    const DistFunc distance_function,
    const ValidActorFunc is_valid_actor = [](Actor&) -> bool { return true; },
    DistType max_distance = std::numeric_limits<DistType>::max()) {
  Actor* best_actor = nullptr;
  DistType best_distance = max_distance;
  for (auto& actor_id : world.active_actors) {
    auto& actor = world.get(actor_id);
    if (!is_valid_actor(actor)) continue;
    const DistType new_distance = distance_function(actor.pos - pos);
    if (new_distance < best_distance) {
      best_distance = new_distance;
      best_actor = &actor;
    }
  }
  return best_actor;
}

template <typename ValidActorFunc>
inline auto get_nearest_actor(
    World& world,
    Position pos,
    const ValidActorFunc is_valid_actor,
    int max_distance_squared = std::numeric_limits<int>::max()) {
  const auto dist_func = [](Position pos) { return euclidean_squared(pos); };
  return get_nearest_actor(world, pos, dist_func, is_valid_actor, max_distance_squared);
}

/// Return a pointer to an Actor at `pos` if it exists.
inline auto actor_at(World& world, Position pos) -> Actor* {
  for (auto& actor_id : world.active_actors) {
    if (world.get(actor_id).pos == pos) return &world.get(actor_id);
  };
  return nullptr;
}

/// Call function (Actor&) -> void on all active actors.
template <typename WithActorFunc>
inline auto with_active_actors(World& world, const WithActorFunc function) {
  for (const auto& actor_id : world.active_actors) function(world.get(actor_id));
}
template <typename WithActorFunc>
inline auto with_active_actors(const World& world, const WithActorFunc function) {
  for (const auto& actor_id : world.active_actors) function(world.get(actor_id));
}

/// Call function (Actor&) -> void on any actors at `pos`.
template <typename WithActorFunc>
inline auto with_actors_at(World& world, Position pos, const WithActorFunc function) {
  with_active_actors(world, [&](Actor& actor) {
    if (actor.pos == pos) function(actor);
  });
}

inline auto freeze_map(World& world, Map& map) -> void {
  for (auto actor_id : world.schedule) {
    if (actor_id == ActorID{0}) continue;  // Is player.
    map.frozen_actors.emplace_back(actor_id);
    world.active_actors.erase(actor_id);
  }
  world.schedule = {ActorID{0}};
}

inline auto find_fixture_by_name(const Map& map, std::string_view name) -> std::optional<Position> {
  for (const auto& [pos, fixture] : map.fixtures) {
    if (fixture.name == name) return pos;
  }
  return {};
}

inline auto activate_map(World& world, Map& map) -> void {
  if (auto found = world.maps.find(world.current_map_id); found != world.maps.end()) freeze_map(world, found->second);
  for (auto actor_id : map.frozen_actors) {
    world.schedule.emplace_back(actor_id);
    world.active_actors.emplace(actor_id);
  }
  map.frozen_actors = {};
  world.current_map_id = map.id;
}
