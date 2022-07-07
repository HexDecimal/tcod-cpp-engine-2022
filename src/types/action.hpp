#pragma once
#include "actor.hpp"
#include "world.hpp"

namespace action {
class Action {
 public:
  virtual ~Action() = default;
  virtual void perform(World& world, Actor& actor) = 0;
};
}  // namespace action
