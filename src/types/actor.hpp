#pragma once
#include <libtcod.hpp>
#include <string>

#include "position.hpp"

struct Actor {
  Position pos;
  std::string name;
  int ch;
  tcod::ColorRGB fg;
};
