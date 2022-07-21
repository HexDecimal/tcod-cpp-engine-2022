#pragma once
#include <fmt/core.h>

#include "../combat.hpp"
#include "base.hpp"

namespace action {
class Pickup : public Action {
 public:
  [[nodiscard]] virtual Result perform(World& world, Actor& actor) override {
    Map& map = world.active_map();
    auto range = map.items.equal_range(actor.pos);
    if (range.first == map.items.end()) {
      return Failure{"Nothing to pickup!"};
    }
    auto item = range.first->second;
    world.log.append("You drink the potion.");
    combat::heal(world, actor, 4);
    map.items.erase(range.first);
    return Success{};
  };

 private:
};

}  // namespace action
