#pragma once
#include "../procgen/caves.hpp"
#include "../types/action.hpp"
#include "../types/actor.hpp"
#include "../types/world.hpp"
#include "../world_logic.hpp"

namespace action {
class UseStairs : public Action {
 public:
  UseStairs() = default;
  UseStairs(bool downwards) : downwards_{downwards} {}
  [[nodiscard]] virtual Result perform(World& world, Actor& actor) override {
    assert(&actor == &world.active_player());
    const Map& current_map = world.active_map();
    if (find_fixture_by_name(current_map, downwards_ ? "down stairs" : "up stairs") != actor.pos) {
      return Failure{downwards_ ? "There are no downward stairs here." : "There are no upward stairs here."};
    }
    const auto dest = MapID{current_map.id.name, current_map.id.level + (downwards_ ? 1 : -1)};
    if (dest.level <= 0) return Failure{"You are already at the top of the caves."};
    Map& next_map = procgen::generate_level(world, dest.level);
    // Perform map movement.
    activate_map(world, next_map);
    actor.pos = find_fixture_by_name(next_map, !downwards_ ? "down stairs" : "up stairs").value();
    return Success{};
  }

 private:
  int downwards_{true};
};
}  // namespace action
