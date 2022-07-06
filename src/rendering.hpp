#pragma once

#include "globals.hpp"

inline void render_map(tcod::Console& console, const Map& map) {
  for (int y{0}; y < map.tiles.get_shape().at(1); ++y) {
    for (int x{0}; x < map.tiles.get_shape().at(0); ++x) {
      if (!console.in_bounds({x, y})) continue;
      if (!map.explored.at({x, y})) continue;
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
inline void render_map(tcod::Console& console, const Map& map, const Actor& player) {
  render_map(console, map);
  if (console.in_bounds(player.pos)) {
    console.at(player.pos) = {player.ch, player.fg, tcod::ColorRGB{0, 0, 0}};
  }
}
inline void render_map() { render_map(g_console, g_world->active_map(), g_world->active_player()); }

inline void main_redraw() {
  g_console.clear();
  if (g_state) g_state->on_draw();
  g_context.present(g_console);
}
