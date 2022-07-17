#pragma once
#include <libtcod.hpp>
#include <memory>
#include <string>

#include "action.hpp"
#include "actor_id.hpp"
#include "position.hpp"
#include "stats.hpp"

struct Actor {
  ActorID id;
  Position pos;
  std::string name;
  int ch;
  tcod::ColorRGB fg;
  Stats stats;
  std::unique_ptr<action::Action> ai;
};
