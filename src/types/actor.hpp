#pragma once
#include <libtcod.hpp>

#include "position.hpp"

struct Actor {
  Position pos;
  int ch;
  tcod::ColorRGB fg;
};
