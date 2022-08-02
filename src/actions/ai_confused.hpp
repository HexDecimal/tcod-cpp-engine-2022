#pragma once
#include "bump.hpp"

namespace action {
class ConfusedAI : public Action {
 public:
  [[nodiscard]] virtual Result perform(World& world, Actor& actor) override {
    const auto random_dir = Position{static_cast<int>(world.rng() % 3) - 1, static_cast<int>(world.rng() % 3) - 1};
    auto result = Bump(random_dir).perform(world, actor);
    if (std::holds_alternative<Failure>(result)) result = Success{};
    return result;
  };
};
}  // namespace action
