#pragma once
#include <unordered_map>

#include "actor.hpp"
#include "map.hpp"

struct World {
  auto active_map() -> Map& { return maps.at("main"); }
  auto active_player() -> Actor& { return actors.at(0); }
  std::vector<Actor> actors;
  std::unordered_map<std::string, Map> maps;
};
