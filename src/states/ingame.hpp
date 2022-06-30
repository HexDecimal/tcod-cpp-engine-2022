#pragma once
#include "../globals.hpp"
#include "../types/state.hpp"

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
    auto& player = g_world->player;
    const int x = player.pos.x + dx;
    const int y = player.pos.y + dy;
    if (!g_world->active_map().tiles.in_bounds({x, y})) return nullptr;
    player.pos = {x, y};
    return nullptr;
  }
  virtual auto on_draw() -> void override {
    auto& map = g_world->active_map();
    for (int y{0}; y < map.tiles.get_shape().at(1); ++y) {
      for (int x{0}; x < map.tiles.get_shape().at(0); ++x) {
        if (!g_console.in_bounds({x, y})) continue;
        g_console.at({x, y}) = map.tiles.at({x, y}) == Tiles::floor
                                   ? TCOD_ConsoleTile{'.', tcod::ColorRGB{64, 64, 64}, tcod::ColorRGB{0, 0, 0}}
                                   : TCOD_ConsoleTile{'#', tcod::ColorRGB{64, 64, 64}, tcod::ColorRGB{0, 0, 0}};
      }
    }
    auto& player = g_world->player;
    if (g_console.in_bounds(player.pos)) {
      g_console.at(player.pos) = {player.ch, player.fg, tcod::ColorRGB{0, 0, 0}};
    }
  }
};
}  // namespace state
