#pragma once

#include <SDL.h>
#include <fmt/core.h>
#include <fmt/format.h>

#include "constants.hpp"
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
  for (const auto& [item_pos, item] : map.items) {
    if (!map.visible.in_bounds(item_pos)) continue;
    if (!map.visible.at(item_pos)) continue;
    if (!console.in_bounds(item_pos)) continue;
    console.at(item_pos).ch = item.ch;
    console.at(item_pos).fg = item.fg;
  }
  for (const auto& [actor_id, actor] : world.actors) {
    if (!map.visible.in_bounds(actor.pos)) continue;
    if (!map.visible.at(actor.pos)) continue;
    if (console.in_bounds(actor.pos)) {
      console.at(actor.pos) = {actor.ch, actor.fg, tcod::ColorRGB{0, 0, 0}};
    }
  }
  if (g_controller.mouse) {
    auto& mouse_pos = *g_controller.mouse;
    if (console.in_bounds(mouse_pos) && map.visible.in_bounds(mouse_pos)) {
      auto& mouse_tile = console.at(mouse_pos);
      mouse_tile = {mouse_tile.ch, tcod::ColorRGB{0, 0, 0}, tcod::ColorRGB{255, 255, 255}};
    }
  }
}
inline void render_map() { render_map(g_console, *g_world); }

inline void render_log(tcod::Console& console, const World& world) {
  const int log_x = 22;
  const int log_width = console.get_width() - log_x;
  const int log_height = console.get_height() - constants::MAP_HEIGHT;
  tcod::Console log_console{log_width, log_height};
  int y = log_console.get_height();
  for (auto it = world.log.messages.crbegin(); it != world.log.messages.crend(); ++it) {
    const auto& msg = *it;
    const auto last_y = y;
    auto print_msg = [&](std::string_view text, const tcod::ColorRGB& fg) {
      y -= tcod::get_height_rect(log_console.get_width(), text);
      tcod::print_rect(log_console, {0, y, 0, log_console.get_width()}, text, fg, {});
    };
    if (msg.count > 1) {
      print_msg(fmt::format("{} (x{})", msg.text, msg.count), msg.fg);
    } else {
      print_msg(msg.text, msg.fg);
    }
    if (y < 0) break;
  }
  tcod::blit(console, log_console, {log_x, 45});
}

inline void draw_bar(
    tcod::Console& console,
    int x,
    int y,
    int width,
    float filled,
    const tcod::ColorRGB fill_color,
    const tcod::ColorRGB back_color) {
  const auto bar_width = std::clamp(static_cast<int>(std::round(width * filled)), 0, width);
  tcod::draw_rect(console, {x, y, width, 1}, 0, {}, back_color);
  tcod::draw_rect(console, {x, y, bar_width, 1}, 0, {}, fill_color);
}

inline void render_mouse_look(tcod::Console& console, const World& world) {
  if (!g_controller.mouse) return;
  const auto& map = world.active_map();
  if (!(map.visible.in_bounds(*g_controller.mouse) && map.visible.at(*g_controller.mouse))) return;
  auto mouse_desc = std::vector<std::string>{};
  for (const auto& [actor_id, actor] : world.actors) {
    if (actor.pos == *g_controller.mouse) {
      mouse_desc.emplace_back(actor.name);
    }
  }
  tcod::print_rect(
      console,
      {1, 0, console.get_width() - 1, 1},
      fmt::format("{}", fmt::join(mouse_desc, ", ")),
      constants::TEXT_COLOR_DEFAULT,
      {});
}

inline void render_gui(tcod::Console& console, const World& world) {
  const auto& player = world.active_player();
  const auto text_color = constants::WHITE;
  const int hp_x = 1;
  const int hp_y = constants::MAP_HEIGHT + 1;

  draw_bar(
      console,
      hp_x,
      hp_y,
      20,
      static_cast<float>(player.stats.hp) / player.stats.max_hp,
      constants::HP_BAR_FILL,
      constants::HP_BAR_BACK);
  tcod::print_rect(
      console, {hp_x, hp_y, 20, 1}, fmt::format(" HP: {}/{}", player.stats.hp, player.stats.max_hp), text_color, {});
  render_log(console, world);
  render_mouse_look(console, world);
}

inline void render_all(tcod::Console& console, const World& world) {
  render_map(console, world);
  render_gui(console, world);
}

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
