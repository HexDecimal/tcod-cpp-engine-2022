#pragma once
#include "../fov.hpp"
#include "../types/action.hpp"
#include "../types/position.hpp"
namespace action {
class Bump : public Action {
 public:
  Bump() = default;
  Bump(Position dir) : dir_{dir} {}
  virtual void perform(World& world, Actor& actor) override {
    const Position dest = {actor.pos.x + dir_.x, actor.pos.y + dir_.y};
    Map& map = world.active_map();
    if (!map.tiles.in_bounds(dest)) return;
    if (map.tiles.at(dest) != Tiles::floor) return;
    actor.pos = dest;
    if (&actor == &world.active_player()) update_fov(map, actor.pos);
  };

 private:
  Position dir_{0, 0};
};

}  // namespace action
