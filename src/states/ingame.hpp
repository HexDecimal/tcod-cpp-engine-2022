#pragma once
#include "../globals.hpp"
#include "../state.hpp"

namespace state {
class InGame : public State {
  virtual auto on_event(SDL_Event& event) -> std::unique_ptr<State> override {
    switch (event.type) {
      case SDL_KEYDOWN: {
        const int modified_y =
            (event.key.keysym.mod & KMOD_SHIFT ? -1 : 0) + (event.key.keysym.mod & KMOD_CTRL ? 1 : 0);
        switch (event.key.keysym.sym) {
          case SDLK_UP:
            return cmd_move(0, -1);
          case SDLK_DOWN:
            return cmd_move(0, 1);
          case SDLK_LEFT:
            return cmd_move(-1, modified_y);
          case SDLK_RIGHT:
            return cmd_move(1, modified_y);
          case SDLK_HOME:
            return cmd_move(-1, -1);
          case SDLK_END:
            return cmd_move(-1, 1);
          case SDLK_PAGEUP:
            return cmd_move(1, -1);
          case SDLK_PAGEDOWN:
            return cmd_move(1, 1);
          case SDLK_KP_1:
            return cmd_move(-1, 1);
          case SDLK_KP_2:
            return cmd_move(0, 1);
          case SDLK_KP_3:
            return cmd_move(1, 1);
          case SDLK_KP_4:
            return cmd_move(-1, 0);
          case SDLK_KP_6:
            return cmd_move(1, 0);
          case SDLK_KP_7:
            return cmd_move(-1, -1);
          case SDLK_KP_8:
            return cmd_move(0, -1);
          case SDLK_KP_9:
            return cmd_move(1, -1);
          default:
            break;
        }
      } break;
      case SDL_QUIT:
        std::exit(EXIT_SUCCESS);
        break;
      default:
        break;
    }
    return nullptr;
  }
  auto cmd_move(int dx, int dy) -> std::unique_ptr<State> {
    player_xy = {player_xy.at(0) + dx, player_xy.at(1) + dy};
    return nullptr;
  }
  virtual auto on_draw() -> void override {
    if (g_console.in_bounds(player_xy)) {
      g_console.at(player_xy) = {'@', tcod::ColorRGB{255, 255, 255}, tcod::ColorRGB{0, 0, 0}};
    }
  }
};
}  // namespace state
