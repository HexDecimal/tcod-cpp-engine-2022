#pragma once

#include <SDL.h>

#include <memory>

namespace state {
class State {
 public:
  virtual ~State() = default;
  [[nodiscard]] virtual auto on_event(SDL_Event& event) -> std::unique_ptr<State> = 0;
  virtual auto on_draw() -> void = 0;
};
}  // namespace state
