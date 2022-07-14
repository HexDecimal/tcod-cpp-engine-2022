#pragma once
#include <cassert>

#include "../actions/bump.hpp"
#include "../fov.hpp"
#include "../globals.hpp"
#include "../logic.hpp"
#include "../mapgen.hpp"
#include "../types/state.hpp"

namespace state {
class InGame : public State {
  virtual auto on_event(SDL_Event& event) -> std::unique_ptr<State> override {
    assert(g_world);
    assert(g_world->schedule.front() == 0);
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
          case SDLK_b:
            return cmd_move(-1, 1);
          case SDLK_KP_2:
          case SDLK_j:
            return cmd_move(0, 1);
          case SDLK_KP_3:
          case SDLK_n:
            return cmd_move(1, 1);
          case SDLK_KP_4:
          case SDLK_h:
            return cmd_move(-1, 0);
          case SDLK_KP_6:
          case SDLK_l:
            return cmd_move(1, 0);
          case SDLK_KP_7:
          case SDLK_y:
            return cmd_move(-1, -1);
          case SDLK_KP_8:
          case SDLK_k:
            return cmd_move(0, -1);
          case SDLK_KP_9:
          case SDLK_u:
            return cmd_move(1, -1);
          case SDLK_F2:
            procgen::generate_level(*g_world);
            return nullptr;
          case SDLK_F3:
            for (auto&& it : g_world->active_map().explored) it = true;
            return nullptr;
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
    action::Bump({dx, dy}).perform(*g_world, g_world->active_player());
    enemy_turn(*g_world);
    return nullptr;
  }
  virtual auto on_draw() -> void override { render_all(g_console, *g_world); }
};
}  // namespace state
