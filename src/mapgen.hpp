#pragma once

#include <fmt/core.h>

#include <random>

#include "actions/basic_ai.hpp"
#include "constants.hpp"
#include "fov.hpp"
#include "maptools.hpp"
#include "rendering.hpp"
#include "types/map.hpp"
#include "types/ndarray.hpp"
#include "types/world.hpp"
#include "world_logic.hpp"

namespace procgen {
/// Call func on the neighbors surrounding x, y.  This may go out of bounds.
template <typename F, typename Adj = std::array<std::array<int, 2>, 8>>
inline void with_neighbors(
    int x,
    int y,
    F func,
    Adj neighbors = std::array<std::array<int, 2>, 8>{
        {{-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {1, 0}, {-1, 1}, {0, 1}, {1, 1}}}) {
  for (const auto adj : neighbors) {
    func(x + adj.at(0), y + adj.at(1));
  }
}
/// Call func(x, y, neighbor_walls) on each tile of the given tiles array.
template <typename Func>
inline void with_tiles_neighbors(const util::Array2D<Tiles>& tiles, Func func) {
  with_indexes(tiles, [&](int x, int y) {
    int walls = 0;
    with_neighbors(x, y, [&](int nx, int ny) {
      if (!tiles.in_bounds({nx, ny})) {
        walls += 1;
        return;
      }
      if (tiles.at({nx, ny}) == Tiles::wall) walls += 1;
    });
    func(x, y, walls);
  });
}

inline void cave_gen_step(Map& map) {
  auto tiles_clone = map.tiles;
  with_tiles_neighbors(tiles_clone, [&](int x, int y, int walls) {
    if (walls < 4) map.tiles.at({x, y}) = Tiles::floor;
    if (walls >= 5) map.tiles.at({x, y}) = Tiles::wall;
  });
}

template <typename T, typename RNG>
inline void shuffle_list(T& sequence, RNG& rng) {
  for (size_t i{0}; i < sequence.size(); ++i) {
    const size_t pick = i + rng() % (sequence.size() - i);
    std::swap(sequence.at(i), sequence.at(pick));
  }
}

inline void shuffle_tiles(World& world, Map& map, const std::vector<Position>& shuffle_space) {
  for (size_t i{0}; i < shuffle_space.size(); ++i) {
    const size_t random_pick = i + world.rng() % (shuffle_space.size() - i);
    std::swap(map.tiles.at(shuffle_space.at(i)), map.tiles.at(shuffle_space.at(random_pick)));
  }
}

inline void cave_gen_ca_shuffle_step(World& world, Map& map) {
  auto shuffle_space = std::vector<Position>{};
  with_tiles_neighbors(map.tiles, [&](int x, int y, int walls) {
    if (map.tiles.at({x, y}) == Tiles::wall && walls < 4) shuffle_space.emplace_back(Position{x, y});
    if (map.tiles.at({x, y}) == Tiles::floor && walls >= 5) shuffle_space.emplace_back(Position{x, y});
  });
  shuffle_tiles(world, map, shuffle_space);
}

inline auto map_label(util::Array2D<bool> tiles) -> std::tuple<util::Array2D<int>, int> {
  auto labels = util::Array2D<int>{tiles.get_shape(), 0};
  auto label_count = int{0};
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
  with_indexes(tiles, [&](int x, int y) {
    if (tiles.at({x, y})) fill_label({x, y}, ++label_count, fill_label);
  });
  return {std::move(labels), label_count};
}

inline auto fill_holes(Map& map) -> void {
  auto is_floor = util::Array2D<bool>{map.tiles.get_shape()};
  std::transform(map.tiles.begin(), map.tiles.end(), is_floor.begin(), [](auto t) { return t == Tiles::floor; });
  const auto [labels, label_n] = map_label(is_floor);
  std::vector<ptrdiff_t> label_sizes;
  label_sizes.reserve(label_n);
  for (int i{0}; i < label_n; ++i) {
    label_sizes.emplace_back(std::count(labels.begin(), labels.end(), i + 1));
  }
  const auto biggest_label =
      static_cast<int>(std::max_element(label_sizes.begin(), label_sizes.end()) - label_sizes.begin()) + 1;
  with_indexes(labels, [&, &labels = labels](int x, int y) {
    if (labels.at({x, y}) && labels.at({x, y}) != biggest_label) {
      map.tiles.at({x, y}) = Tiles::wall;
    }
  });
  fmt::print("Filled {} holes.\n", label_n - 1);
}

/// Pop and return a random item from a vector.
template <typename VectorLike, typename RNG>
inline auto pop_random(VectorLike& list, RNG& rng) {
  auto pop_iter = list.begin() + rng() % list.size();
  auto item = std::move(*pop_iter);
  list.erase(pop_iter);
  return item;
}

inline auto generate_level(World& world) -> Map& {
  const int WIDTH = 80;
  const int HEIGHT = 45;
  world.rng = std::mt19937(std::rand() ^ static_cast<uint32_t>(std::time(nullptr)));
  auto& map = world.maps["main"] = Map{WIDTH, HEIGHT};
  debug_show_map(map);
  for (size_t i{}; i < map.tiles.get_container().size(); ++i) {
    map.tiles.get_container().at(i) = (i < map.tiles.get_container().size() * 45 / 100 ? Tiles::wall : Tiles::floor);
  }
  debug_show_map(map);
  shuffle_list(map.tiles.get_container(), world.rng);
  debug_show_map(map);
  for (int repeats{0}; repeats < 5; ++repeats) {
    cave_gen_ca_shuffle_step(world, map);
    debug_show_map(map);
  }
  with_border(WIDTH, HEIGHT, [&](int x, int y) { map.tiles.at({x, y}) = Tiles::wall; });
  fill_holes(map);
  debug_show_map(map);

  auto floor_tiles = std::vector<Position>{};
  floor_tiles.reserve(WIDTH * HEIGHT);
  with_indexes(map, [&](int x, int y) {
    if (map.tiles.at({x, y}) == Tiles::floor) floor_tiles.emplace_back(Position{x, y});
  });

  auto& player = world.active_player();
  player.pos = pop_random(floor_tiles, world.rng);
  update_fov(map, player.pos);

  for (int repeats{0}; repeats < 5; ++repeats) {
    auto new_item = Item{};
    new_item.name = "health potion";
    new_item.fg = tcod::ColorRGB{128, 21, 21};
    new_item.ch = '!';
    map.items.emplace(pop_random(floor_tiles, world.rng), new_item);
  }

  // Remove tiles in FOV.
  floor_tiles.erase(
      std::remove_if(floor_tiles.begin(), floor_tiles.end(), [&](Position pos) { return map.visible.at(pos); }),
      floor_tiles.end());

  for (int repeats{0}; repeats < 20; ++repeats) {
    auto& [monster_id, monster] = *new_actor(world);
    monster.pos = pop_random(floor_tiles, world.rng);
    monster.name = "orc";
    monster.ch = 'o';
    monster.fg = {63, 127, 63};
    monster.stats.max_hp = monster.stats.hp = 10;
    monster.stats.attack = 3;
    monster.ai = std::make_unique<action::BasicAI>();
    world.schedule.push_back(monster_id);
  }

  return map;
}
}  //  namespace procgen
