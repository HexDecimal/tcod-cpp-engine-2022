#pragma once
#include <libtcod.hpp>

#include "types/map.hpp"
#include "types/position.hpp"

inline auto update_fov(Map& map, Position pov) {
  const auto [WIDTH, HEIGHT] = map.get_size();
  auto fov_map = TCODMap{WIDTH, HEIGHT};
  for (int y{0}; y < HEIGHT; ++y) {
    for (int x{0}; x < WIDTH; ++x) {
      fov_map.setProperties(x, y, map.tiles.at({x, y}) == Tiles::floor, false);
    }
  }
  fov_map.computeFov(pov.x, pov.y, 8, true, FOV_SYMMETRIC_SHADOWCAST);
  for (int y{0}; y < HEIGHT; ++y) {
    for (int x{0}; x < WIDTH; ++x) {
      map.visible.at({x, y}) = fov_map.isInFov(x, y);
      map.explored.at({x, y}) = map.explored.at({x, y}) || map.visible.at({x, y});
    }
  }
}
