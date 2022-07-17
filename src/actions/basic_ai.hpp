#pragma once
#include "../types/position.hpp"
#include "base.hpp"
#include "bump.hpp"

namespace action {
class BasicAI : public Action {
 public:
  virtual void perform(World& world, Actor& actor) override {
    Map& map = world.active_map();
    const auto can_see_player = map.visible.at(actor.pos);
    auto move_dir = world.active_player().pos - actor.pos;
    move_dir.x = sign_(move_dir.x);
    move_dir.y = sign_(move_dir.y);
    return Bump(move_dir).perform(world, actor);
  };

 private:
  static auto sign_(int n) -> int { return (n == 0 ? 0 : (n < 0 ? -1 : 1)); }
};

}  // namespace action
