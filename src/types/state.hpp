#pragma once

#include <SDL.h>

#include <memory>

namespace state {
class State {
 public:
  using StateReturnType = std::unique_ptr<State>;
  virtual ~State() = default;
  [[nodiscard]] virtual auto on_event(SDL_Event& event) -> StateReturnType = 0;
  virtual auto on_draw() -> void = 0;
};
}  // namespace state
