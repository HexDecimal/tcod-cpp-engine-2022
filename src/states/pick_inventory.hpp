#pragma once
#include <cassert>
#include <functional>
#include <libtcod.hpp>

#include "../actions/use_item.hpp"
#include "../globals.hpp"
#include "../types/state.hpp"

namespace state {
class PickInventory : public State {
 public:
  using PickFunction = std::function<StateReturnType(int)>;
  PickInventory(std::unique_ptr<State> parent, PickFunction on_pick) : parent_{std::move(parent)}, on_pick_{on_pick} {};
  virtual auto on_event(SDL_Event& event) -> StateReturnType override {
    assert(g_world);
    switch (event.type) {
      case SDL_KEYDOWN:
        switch (event.key.keysym.sym) {
          case SDLK_ESCAPE:
            return Change{std::move(parent_)};
          default:
            auto sym = event.key.keysym.sym;
            if (SDLK_a <= sym && sym <= SDLK_z) {
              return on_pick_(sym - SDLK_a);
            }
            break;
        }
        break;
      case SDL_QUIT:
        return Quit{};
      default:
        break;
    }
    return {};
  }
  virtual auto on_draw() -> void override {
    parent_->on_draw();
    static constexpr auto INVENTORY_WIDTH = 50;
    static constexpr auto INVENTORY_HEIGHT = 28;
    static constexpr auto DECORATION =
        std::array<int, 9>{0x250c, 0x2500, 0x2510, 0x2502, 0, 0x2502, 0x2514, 0x2500, 0x2518};
    auto console = tcod::Console{INVENTORY_WIDTH, INVENTORY_HEIGHT};
    tcod::draw_frame(console, {0, 0, console.get_width(), console.get_height()}, DECORATION, {}, {});
    tcod::print(console, {2, 0}, "┤Inventory├", {}, {});
    tcod::print(
        console,
        {2, console.get_height() - 1},
        "[a-z]Use Item, [ESC]Cancel",
        tcod::ColorRGB{0, 0, 0},
        tcod::ColorRGB{255, 255, 255});
    if (g_world->active_player().stats.inventory.size()) {
      int shortcut = 'a';
      int y = 1;
      for (const auto& item : g_world->active_player().stats.inventory) {
        tcod::print(
            console, {1, y++}, fmt::format("({:c}) {} ({})", shortcut++, item->get_name(), item->count), {}, {});
      }
    } else {
      tcod::print(console, {1, 1}, "You have no items.", {}, {});
    }
    tcod::blit(
        g_console,
        console,
        {g_console.get_width() / 2 - console.get_width() / 2, g_console.get_height() / 2 - console.get_height() / 2});
  }

 private:
  std::unique_ptr<State> parent_;
  PickFunction on_pick_;
};
}  // namespace state
