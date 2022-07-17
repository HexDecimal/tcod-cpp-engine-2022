#pragma once
#include <fmt/core.h>

#include "globals.hpp"
#include "logic.hpp"
#include "states/dead.hpp"
#include "types/actor.hpp"
#include "types/world.hpp"

namespace combat {
inline auto kill(World& world, Actor& target) {
  if (target.id == 0) {
    world.log.append(fmt::format("You have died!"));
    g_state = std::make_unique<state::Dead>();
  } else {
    world.log.append(fmt::format("The {} dies!", target.name));
    world.actors.erase(target.id);
  }
}

inline auto attack(World& world, Actor& self, Actor& target) {
  auto damage = self.stats.attack;
  damage -= target.stats.defense;
  target.stats.hp -= damage;
  damage = std::max(0, damage);
  if (damage > 0) {
    world.log.append(fmt::format("{} attacks {} for {} hit points.", self.name, target.name, damage));
  } else {
    world.log.append(fmt::format("{} attacks {} but it has no effect!", self.name, target.name));
  }
  if (target.stats.hp <= 0) {
    kill(world, target);
  };
}
}  // namespace combat
