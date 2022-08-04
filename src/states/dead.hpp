#pragma once
#include <cassert>
#include <filesystem>

#include "../rendering.hpp"
#include "../types/state.hpp"
#include "main_menu.hpp"

namespace state {
class Dead : public State {
  virtual auto on_event(SDL_Event& event) -> StateReturnType override {
    assert(g_world);
    switch (event.type) {
      case SDL_KEYDOWN:
        switch (event.key.keysym.sym) {
          case SDLK_ESCAPE:
            std::filesystem::remove("saves/save.json");
            g_world = nullptr;
            return Change{std::make_unique<MainMenu>()};
            break;
          default:
            break;
        }
        break;
      case SDL_QUIT:
        std::filesystem::remove("saves/save.json");
        return Quit{};
      default:
        break;
    }
    return {};
  }
  virtual auto on_draw() -> void override { render_all(g_console, *g_world); }
};
}  // namespace state
