#pragma once
#include <libtcod/color.hpp>
#include <string>

#include "action_result.hpp"
#include "actor_fwd.hpp"
#include "position.hpp"
#include "world_fwd.hpp"

struct Item {
  virtual ~Item() = default;
  [[nodiscard]] virtual std::string get_name() const { return name; }
  [[nodiscard]] virtual std::tuple<int, tcod::ColorRGB> get_graphic() const { return {ch, fg}; }
  [[nodiscard]] virtual action::Result use_item(World& world, Actor& actor) = 0;
  std::string name = "<undefined>";
  int ch = '?';
  tcod::ColorRGB fg = {255, 255, 255};
  int count = 1;
};
