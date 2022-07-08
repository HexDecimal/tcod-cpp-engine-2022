#pragma once
#include <cassert>
#include <libtcod.hpp>

enum class Tiles {
  wall = 0,
  floor,
};

struct Map {
  Map() = default;
  Map(int width, int height) : tiles{{width, height}}, explored{{width, height}}, visible{{width, height}} {}
  /// Return the [width, height] of this map.
  auto get_size() const noexcept -> std::array<int, 2> {
    assert(tiles.get_shape() == explored.get_shape());
    assert(tiles.get_shape() == visible.get_shape());
    return tiles.get_shape();
  }
  auto get_width() const noexcept -> int { return get_size().at(0); }
  auto get_height() const noexcept -> int { return get_size().at(1); }
  tcod::Matrix<Tiles, 2> tiles;
  tcod::Matrix<bool, 2> explored;
  tcod::Matrix<bool, 2> visible;
};
