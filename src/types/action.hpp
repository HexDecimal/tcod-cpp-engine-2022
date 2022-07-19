#pragma once
#include "action_result.hpp"
#include "actor_fwd.hpp"
#include "world_fwd.hpp"

namespace action {
class Action {
 public:
  virtual ~Action() = default;
  [[nodiscard]] virtual Result perform(World& world, Actor& actor) = 0;
};
}  // namespace action
