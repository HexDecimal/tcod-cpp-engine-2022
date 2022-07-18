#pragma once

#include <libtcod/color.hpp>

namespace constants {
constexpr auto CONSOLE_WIDTH = 80;
constexpr auto CONSOLE_HEIGHT = 50;
constexpr auto MAP_WIDTH = 80;
constexpr auto MAP_HEIGHT = 45;
constexpr auto TEXT_COLOR_DEFAULT = tcod::ColorRGB{128, 128, 128};
constexpr auto WHITE = tcod::ColorRGB{255, 255, 255};

// https://paletton.com/#uid=1000u0kllllaFw0g0qFqFg0w0aF
constexpr auto HP_BAR_BACK = tcod::ColorRGB{85, 0, 0};
constexpr auto HP_BAR_FILL = tcod::ColorRGB{128, 21, 21};
}  // namespace constants
