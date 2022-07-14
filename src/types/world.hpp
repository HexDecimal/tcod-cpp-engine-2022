#pragma once
#include <deque>
#include <random>
#include <unordered_map>

#include "actor.hpp"
#include "map.hpp"
#include "messages.hpp"

using ActorID = int;

struct World {
  auto active_map() -> Map& { return maps.at("main"); }
  auto active_map() const -> const Map& { return maps.at("main"); }
  auto active_player() -> Actor& { return actors.at(0); }
  auto active_player() const -> const Actor& { return actors.at(0); }
  std::unordered_map<ActorID, Actor> actors;
  std::unordered_map<std::string, Map> maps;
  std::mt19937 rng;
  std::deque<ActorID> schedule;
  MessageLog log;
};
