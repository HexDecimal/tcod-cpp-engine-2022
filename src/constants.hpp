#pragma once

#include <libtcod/color.hpp>

namespace constants {
constexpr auto CONSOLE_WIDTH = 80;
constexpr auto CONSOLE_HEIGHT = 50;
constexpr auto MAP_WIDTH = 80;
constexpr auto MAP_HEIGHT = 45;
constexpr auto TEXT_COLOR_DEFAULT = tcod::ColorRGB{128, 128, 128};
constexpr auto MENU_COLOR_DEFAULT = TEXT_COLOR_DEFAULT;
constexpr auto MENU_COLOR_HIGHLIGHT = tcod::ColorRGB{255, 191, 127};
constexpr auto WHITE = tcod::ColorRGB{255, 255, 255};

// https://paletton.com/#uid=1000u0kllllaFw0g0qFqFg0w0aF
constexpr auto HP_BAR_BACK = tcod::ColorRGB{85, 0, 0};
constexpr auto HP_BAR_FILL = tcod::ColorRGB{128, 21, 21};
// https://paletton.com/#uid=14H0u0knjpZdTAzj6tnqPlGv4gS
constexpr auto XP_BAR_BACK = tcod::ColorRGB{61, 7, 90};
constexpr auto XP_BAR_FILL = tcod::ColorRGB{83, 24, 115};
}  // namespace constants
