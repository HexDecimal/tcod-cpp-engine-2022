#pragma once
#include <libtcod/color.hpp>
#include <string>

struct Fixture {
  std::string name;
  int ch;
  tcod::ColorRGB fg = {255, 255, 255};
};
