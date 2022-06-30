#pragma once
#include <libtcod.hpp>

enum class Tiles {
  wall = 0,
  floor,
};

struct Map {
  Map() = default;
  Map(int width, int height) : tiles{{width, height}} {}
  tcod::Matrix<Tiles, 2> tiles;
};
