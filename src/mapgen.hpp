#pragma once

#include <iostream>
#include <random>

#include "fov.hpp"
#include "types/map.hpp"
#include "types/world.hpp"

namespace procgen {
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

inline auto map_label(tcod::Matrix<bool, 2> tiles) -> std::tuple<tcod::Matrix<int, 2>, int> {
  auto labels = tcod::Matrix<int, 2>{tiles.get_shape(), 0};
  auto label_count = int{0};
  const auto [WIDTH, HEIGHT] = tiles.get_shape();
  auto fill_label = [&](std::array<int, 2> xy, int label_i, auto& recursive) -> void {
    if (!tiles.in_bounds(xy)) return;
    if (!tiles.at(xy)) return;
    tiles.at(xy) = false;
    labels.at(xy) = label_i;
    const auto [x, y] = xy;
    recursive({x, y - 1}, label_i, recursive);
    recursive({x - 1, y}, label_i, recursive);
    recursive({x + 1, y}, label_i, recursive);
    recursive({x, y + 1}, label_i, recursive);
  };
  for (int y{0}; y < HEIGHT; ++y) {
    for (int x{0}; x < WIDTH; ++x) {
      if (tiles.at({x, y})) {
        fill_label({x, y}, ++label_count, fill_label);
      }
    }
  }
  return {std::move(labels), label_count};
}

inline auto fill_holes(Map& map) -> void {
  const auto [WIDTH, HEIGHT] = map.tiles.get_shape();
  auto is_floor = tcod::Matrix<bool, 2>{map.tiles.get_shape()};
  std::transform(map.tiles.begin(), map.tiles.end(), is_floor.begin(), [](auto t) { return t == Tiles::floor; });
  const auto [labels, label_n] = map_label(is_floor);
  std::vector<ptrdiff_t> label_sizes;
  for (int i{0}; i < label_n; ++i) {
    label_sizes.emplace_back(std::count(labels.begin(), labels.end(), i + 1));
  }
  const auto biggest_label =
      static_cast<int>(std::max_element(label_sizes.begin(), label_sizes.end()) - label_sizes.begin()) + 1;
  for (int y{0}; y < HEIGHT; ++y) {
    for (int x{0}; x < WIDTH; ++x) {
      if (labels.at({x, y}) && labels.at({x, y}) != biggest_label) {
        map.tiles.at({x, y}) = Tiles::wall;
      }
    }
  }
  std::cout << "Filled " << (label_n - 1) << " holes.\n";
}

inline auto generate_level(World& world) -> Map& {
  const int WIDTH = 80;
  const int HEIGHT = 45;
  auto rng = std::mt19937(std::rand() ^ static_cast<uint32_t>(std::time(nullptr)));
  auto rng_random = std::uniform_real_distribution(0.0f, 1.0f);
  auto& map = world.maps["main"] = Map{WIDTH, HEIGHT};
  for (auto& it : map.tiles) it = Tiles::floor;
  for (auto& it : map.tiles) it = rng_random(rng) > 0.45f ? Tiles::floor : Tiles::wall;
  cave_gen_step(map);
  cave_gen_step(map);
  cave_gen_step(map);
  cave_gen_step(map);
  with_border(WIDTH, HEIGHT, [&](int x, int y) { map.tiles.at({x, y}) = Tiles::wall; });
  fill_holes(map);

  std::vector<std::array<int, 2>> floor_tiles;
  floor_tiles.reserve(std::count(map.tiles.begin(), map.tiles.end(), Tiles::floor));
  for (int y{0}; y < HEIGHT; ++y) {
    for (int x{0}; x < WIDTH; ++x) {
      if (map.tiles.at({x, y}) == Tiles::floor) floor_tiles.emplace_back(std::array<int, 2>{x, y});
    }
  }
  const auto player_xy = floor_tiles.at(std::uniform_int_distribution<intptr_t>(0, floor_tiles.size())(rng));
  world.player.pos = {player_xy.at(0), player_xy.at(1)};
  update_fov(map, world.player.pos);

  return map;
}
}  //  namespace procgen
