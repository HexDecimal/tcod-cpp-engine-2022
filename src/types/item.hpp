#pragma once
#include <libtcod/color.hpp>
#include <string>

#include "position.hpp"

struct Item {
  std::string name;
  int ch;
  tcod::ColorRGB fg;
  int count = 1;
};
