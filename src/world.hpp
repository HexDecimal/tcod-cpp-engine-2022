#pragma once
#include "actor.hpp"
#include "map.hpp"

struct World {
  Map map;
  Actor player;
};
