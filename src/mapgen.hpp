#pragma once

#include <random>

#include "types/map.hpp"
#include "types/world.hpp"

template <typename Func>
inline void with_border(int width, int height, const Func& func) {
  for (int y{0}; y < height; ++y) {
    for (int x{0}; x < width; ++x) {
      if (x == 0 || y == 0 || x == width - 1 || y == height - 1) func(x, y);
    }
  }
}

inline void cave_gen_step(Map& map) {
  const auto [WIDTH, HEIGHT] = map.tiles.get_shape();
  const auto tiles_clone = map.tiles;
  constexpr const std::array<std::array<int, 2>, 8> NEIGHBORS{
      {{-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {1, 0}, {-1, 1}, {0, 1}, {1, 1}}};
  for (int y{0}; y < HEIGHT; ++y) {
    for (int x{0}; x < WIDTH; ++x) {
      int walls = 0;
      for (const auto adj : NEIGHBORS) {
        const auto nx = x + adj[0];
        const auto ny = y + adj[1];
        if (!tiles_clone.in_bounds({nx, ny})) {
          walls += 1;
          continue;
        }
        if (tiles_clone.at({nx, ny}) == Tiles::wall) walls += 1;
      }
      if (walls < 4) map.tiles.at({x, y}) = Tiles::floor;
      if (walls >= 5) map.tiles.at({x, y}) = Tiles::wall;
    }
  }
}

inline auto generate_level(World& world) -> Map& {
  const int WIDTH = 80;
  const int HEIGHT = 45;
  auto rng = std::mt19937(42);
  auto rng_random = std::uniform_real_distribution(0.0f, 1.0f);
  auto& map = world.maps["main"] = Map{WIDTH, HEIGHT};
  for (auto& it : map.tiles) it = Tiles::floor;
  for (auto& it : map.tiles) it = rng_random(rng) > 0.45f ? Tiles::floor : Tiles::wall;
  cave_gen_step(map);
  cave_gen_step(map);
  cave_gen_step(map);
  cave_gen_step(map);
  with_border(WIDTH, HEIGHT, [&](int x, int y) { map.tiles.at({x, y}) = Tiles::wall; });
  return map;
}
