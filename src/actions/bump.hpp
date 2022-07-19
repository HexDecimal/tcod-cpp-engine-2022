#pragma once
#include <fmt/core.h>

#include "../combat.hpp"
#include "../fov.hpp"
#include "../types/position.hpp"
#include "base.hpp"
namespace action {
class Bump : public Action {
 public:
  Bump() = default;
  Bump(Position dir) : dir_{dir} {}
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
    for (auto& [other_id, other] : world.actors) {
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
