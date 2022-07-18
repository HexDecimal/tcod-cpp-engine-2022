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
  virtual void perform(World& world, Actor& actor) override {
    const Position dest = actor.pos + dir_;
    Map& map = world.active_map();
    if (!map.tiles.in_bounds(dest)) {
      if (actor.id == 0) world.log.append("That way is blocked!");
      return;
    }
    if (map.tiles.at(dest) != Tiles::floor) {
      if (actor.id == 0) world.log.append("That way is blocked!");
      return;
    }
    for (auto& [other_id, other] : world.actors) {
      if (other.pos == dest) {
        combat::attack(world, actor, other);
        return;
      }
    }
    actor.pos = dest;
    if (&actor == &world.active_player()) update_fov(map, actor.pos);
  };

 private:
  Position dir_{0, 0};
};

}  // namespace action
