#pragma once
#include "actor_fwd.hpp"
#include "world_fwd.hpp"

namespace action {
class Action {
 public:
  virtual ~Action() = default;
  virtual void perform(World& world, Actor& actor) = 0;
};
}  // namespace action
