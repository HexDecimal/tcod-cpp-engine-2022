#pragma once
#include <functional>
#include <gsl/gsl>
#include <vector>

#include "../constants.hpp"
#include "../globals.hpp"
#include "../types/position.hpp"
#include "../types/state.hpp"

namespace state {
class Menu : public State {
 public:
  struct Item {
    std::string name;  // The displayed text of this item.
    std::function<StateReturnType()> on_pick;  // The callback for when this item is picked.
    int key = 0;  // The SDL_KeyCode to use as the hotkey for this item.
  };
  using MenuItems = std::vector<Item>;
  explicit Menu(MenuItems items = {}, int selected = 0) : items_{std::move(items)}, selected_{selected} {}

  virtual auto on_event(SDL_Event& event) -> StateReturnType override {
    switch (event.type) {
      case SDL_KEYDOWN:
        switch (event.key.keysym.sym) {
          case SDLK_UP:
            if (!get_selected_item()) selected_ = static_cast<int>(items_.size());
            selected_ -= 1;
            if (selected_ == -1) selected_ += static_cast<int>(items_.size());
            break;
          case SDLK_DOWN:
            if (!get_selected_item()) selected_ = -1;
            selected_ = (selected_ + 1) % items_.size();
            break;
          case SDLK_RETURN:
          case SDLK_RETURN2:
          case SDLK_KP_ENTER: {
            const auto* item = get_selected_item();
            if (item) return item->on_pick();
            break;
          }
          default:
            for (size_t i{0}; i < items_.size(); ++i) {
              if (items_.at(i).key && items_.at(i).key == event.key.keysym.sym) {
                selected_ = gsl::narrow<int>(i);
                return get_selected_item()->on_pick();
              }
            }
            break;
        }
        break;
      case SDL_MOUSEMOTION:
        g_context.convert_event_coordinates(event);
        selected_ = event.motion.y - menu_pos_.y;
        break;
      case SDL_MOUSEBUTTONUP:
        if (event.button.button == 1) {
          const auto* item = get_selected_item();
          if (item) return item->on_pick();
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
    for (int i{0}; i < static_cast<int>(items_.size()); ++i) {
      const auto& item = items_.at(i);
      const auto is_selected = (i == selected_);
      const auto fg = is_selected ? constants::MENU_COLOR_HIGHLIGHT : constants::MENU_COLOR_DEFAULT;
      tcod::print(g_console, {menu_pos_.x, menu_pos_.y + i}, item.name, fg, tcod::ColorRGB{0, 0, 0});
    }
  }

  [[nodiscard]] auto get_selected_item() const -> const Item* {
    if (0 <= selected_ && selected_ < static_cast<int>(items_.size())) return &items_.at(selected_);
    return nullptr;
  }

  MenuItems items_{};
  int selected_ = 0;
  static const constexpr Position menu_pos_{10, 10};
};
}  // namespace state
