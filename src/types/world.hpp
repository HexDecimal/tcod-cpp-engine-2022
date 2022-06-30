#pragma once
#include <unordered_map>

#include "actor.hpp"
#include "map.hpp"

struct World {
  auto active_map() -> Map& { return maps.at("main"); }
  Actor player;
  std::unordered_map<std::string, Map> maps;
};
