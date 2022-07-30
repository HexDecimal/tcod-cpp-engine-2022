#pragma once
#include <SDL_events.h>

#include <optional>

#include "types/position.hpp"

/// Return a direction that this event corresponds to.
inline auto get_dir_from(const SDL_Event& event) -> std::optional<Position> {
  switch (event.type) {
    case SDL_KEYDOWN: {
      const int modified_y = (event.key.keysym.mod & KMOD_SHIFT ? -1 : 0) + (event.key.keysym.mod & KMOD_CTRL ? 1 : 0);
      switch (event.key.keysym.sym) {
        case SDLK_PERIOD:
        case SDLK_KP_5:
        case SDLK_KP_CLEAR:
          return Position{0, 0};
        case SDLK_UP:
          return Position{0, -1};
        case SDLK_DOWN:
          return Position{0, 1};
        case SDLK_LEFT:
          return Position{-1, modified_y};
        case SDLK_RIGHT:
          return Position{1, modified_y};
        case SDLK_HOME:
          return Position{-1, -1};
        case SDLK_END:
          return Position{-1, 1};
        case SDLK_PAGEUP:
          return Position{1, -1};
        case SDLK_PAGEDOWN:
          return Position{1, 1};
        case SDLK_KP_1:
        case SDLK_b:
          return Position{-1, 1};
        case SDLK_KP_2:
        case SDLK_j:
          return Position{0, 1};
        case SDLK_KP_3:
        case SDLK_n:
          return Position{1, 1};
        case SDLK_KP_4:
        case SDLK_h:
          return Position{-1, 0};
        case SDLK_KP_6:
        case SDLK_l:
          return Position{1, 0};
        case SDLK_KP_7:
        case SDLK_y:
          return Position{-1, -1};
        case SDLK_KP_8:
        case SDLK_k:
          return Position{0, -1};
        case SDLK_KP_9:
        case SDLK_u:
          return Position{1, -1};
        default:
          break;
      }
    }
    default:
      break;
  }
  return {};
}
