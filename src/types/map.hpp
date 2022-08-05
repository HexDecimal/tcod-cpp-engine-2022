#pragma once
#include <cassert>
#include <libtcod.hpp>
#include <unordered_map>

#include "actor_id.hpp"
#include "fixture.hpp"
#include "item.hpp"
#include "map_id.hpp"
#include "ndarray.hpp"
#include "position.hpp"

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

  MapID id;
  util::Array2D<Tiles> tiles;
  util::Array2D<bool> explored;
  util::Array2D<bool> visible;
  std::unordered_multimap<Position, std::unique_ptr<Item>> items;
  std::unordered_map<Position, Fixture> fixtures;
  std::vector<ActorID> frozen_actors;
};
