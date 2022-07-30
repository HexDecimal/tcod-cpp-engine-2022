#pragma once

#include <SDL_events.h>

#include <memory>

#include "state_result.hpp"

namespace state {
class State {
 public:
  using StateReturnType = Result;
  virtual ~State() = default;
  [[nodiscard]] virtual auto on_event(SDL_Event& event) -> StateReturnType = 0;
  virtual auto on_draw() -> void = 0;
};
}  // namespace state
