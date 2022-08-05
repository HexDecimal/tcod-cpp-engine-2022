#pragma once
#include <deque>
#include <random>
#include <unordered_map>

#include "actor.hpp"
#include "actor_id.hpp"
#include "map.hpp"
#include "messages.hpp"

struct World {
  auto active_map() -> Map& { return maps.at(current_map_id); }
  auto active_map() const -> const Map& { return maps.at(current_map_id); }
  auto active_player() -> Actor& { return actors.at(0); }
  auto active_player() const -> const Actor& { return actors.at(0); }
  std::unordered_map<ActorID, Actor> actors;
  std::unordered_map<MapID, Map> maps;
  std::mt19937 rng;
  std::deque<ActorID> schedule;
  MessageLog log;
  MapID current_map_id = {"", 0};
};
