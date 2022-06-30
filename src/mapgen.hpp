#pragma once

#include "types/map.hpp"
#include "types/world.hpp"

inline auto generate_level(World& world) -> Map& {
  const int WIDTH = 80;
  const int HEIGHT = 45;
  auto& map = world.maps["main"] = Map{WIDTH, HEIGHT};
  for (int y{0}; y < HEIGHT; ++y) {
    for (int x{0}; x < WIDTH; ++x) {
      const auto is_border = (x == 0 || y == 0 || x == WIDTH - 1 || y == HEIGHT - 1);
      map.tiles.at({x, y}) = is_border ? Tiles::wall : Tiles::floor;
    }
  }
  return map;
}
