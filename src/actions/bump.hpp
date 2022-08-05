#pragma once
#include <fmt/core.h>

#include <algorithm>

#include "../combat.hpp"
#include "../fov.hpp"
#include "../types/position.hpp"
#include "base.hpp"

namespace action {
class Bump : public Action {
 public:
  Bump() = default;
  Bump(Position dir) : dir_{dir} {
    assert(0 <= std::abs(dir_.x));
    assert(std::abs(dir_.x) <= 1);
    assert(0 <= std::abs(dir_.y));
    assert(std::abs(dir_.y) <= 1);
  }
  [[nodiscard]] virtual Result perform(World& world, Actor& actor) override {
    if (dir_ == Position{0, 0}) return Success{};
    const Position dest = actor.pos + dir_;
    Map& map = world.active_map();
    if (!map.tiles.in_bounds(dest)) {
      return Failure{"That way is blocked!"};
    }
    if (map.tiles.at(dest) != Tiles::floor) {
      return Failure{"That way is blocked!"};
    }
    for (auto& other_id : world.active_actors) {
      auto& other = world.get(other_id);
      if (other.pos == dest) {
        combat::attack(world, actor, other);
        return Success{};
      }
    }
    actor.pos = dest;
    if (&actor == &world.active_player()) update_fov(map, actor.pos);
    return Success{};
  };

 private:
  Position dir_{0, 0};
};

}  // namespace action
