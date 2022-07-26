#pragma once
#include <cassert>

#include "../rendering.hpp"
#include "../types/state.hpp"

namespace state {
class Dead : public State {
  virtual auto on_event(SDL_Event& event) -> StateReturnType override {
    assert(g_world);
    switch (event.type) {
      case SDL_QUIT:
        std::exit(EXIT_SUCCESS);
        break;
      default:
        break;
    }
    return nullptr;
  }
  virtual auto on_draw() -> void override { render_all(g_console, *g_world); }
};
}  // namespace state
