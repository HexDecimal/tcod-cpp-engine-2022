#pragma once
#include <libtcod.hpp>

enum class Tiles {
  floor = 0,
  wall,
};

struct Map {
  Map() = default;
  Map(int width, int height) : tiles{{width, height}} {}
  tcod::Matrix<Tiles, 2> tiles;
};
