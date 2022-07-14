#pragma once

#include <SDL.h>

#include "globals.hpp"

inline void render_map(tcod::Console& console, const Map& map, bool show_all = false) {
  for (int y{0}; y < map.get_height(); ++y) {
    for (int x{0}; x < map.get_width(); ++x) {
      if (!console.in_bounds({x, y})) continue;
      if (!show_all && !map.explored.at({x, y})) continue;
      console.at({x, y}) = map.tiles.at({x, y}) == Tiles::floor
                               ? TCOD_ConsoleTile{'.', tcod::ColorRGB{128, 128, 128}, tcod::ColorRGB{0, 0, 0}}
                               : TCOD_ConsoleTile{'#', tcod::ColorRGB{128, 128, 128}, tcod::ColorRGB{0, 0, 0}};
      if (!map.visible.at({x, y})) {
        console.at({x, y}).fg.r /= 2;
        console.at({x, y}).fg.g /= 2;
        console.at({x, y}).fg.b /= 2;
        console.at({x, y}).bg.r /= 2;
        console.at({x, y}).bg.g /= 2;
        console.at({x, y}).bg.b /= 2;
      }
    }
  }
}
inline void render_map(tcod::Console& console, const World& world) {
  const auto& map = world.active_map();
  render_map(console, map);
  for (const auto& [actor_id, actor] : world.actors) {
    if (!map.visible.in_bounds(actor.pos)) continue;
    if (!map.visible.at(actor.pos)) continue;
    if (console.in_bounds(actor.pos)) {
      console.at(actor.pos) = {actor.ch, actor.fg, tcod::ColorRGB{0, 0, 0}};
    }
  }
}
inline void render_map() { render_map(g_console, *g_world); }

inline void main_redraw() {
  g_console.clear();
  if (g_state) g_state->on_draw();
  g_context.present(g_console);
}

inline auto debug_show_map(const Map& map) {
  static int n = 0;
#ifndef NDEBUG
  render_map(g_console, map, true);
  g_context.present(g_console);
  SDL_Delay(100);
#endif
}
