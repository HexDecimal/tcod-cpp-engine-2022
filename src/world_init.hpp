#pragma once
#include <fmt/core.h>

#include "mapgen.hpp"
#include "types/actor.hpp"
#include "types/world.hpp"

inline auto new_world() -> std::unique_ptr<World> {
  auto world = std::make_unique<World>();
  world->log.append(
      "Welcome stranger!\nPrepare to perish in the Tombs of the Ancient Kings.", tcod::ColorRGB{212, 106, 106});
  auto& player = world->actors.emplace(0, Actor{}).first->second;
  player.name = "player";
  player.ch = '@';
  player.fg = {255, 255, 255};
  player.stats.max_hp = player.stats.hp = 30;
  player.stats.attack = 5;
  player.stats.defense = 2;

  player.stats.inventory.emplace_back(Item{"scroll of lightning bolt", '#', tcod::ColorRGB{TCODColor::lightYellow}});

  world->schedule.emplace_back(0);

  procgen::generate_level(*world);
  return world;
}
