#pragma once
#include <fmt/core.h>

#include "globals.hpp"
#include "states/dead.hpp"
#include "types/actor.hpp"
#include "types/world.hpp"
#include "world_logic.hpp"

namespace combat {
inline auto kill(World& world, Actor& target) {
  if (target.id == 0) {
    world.log.append(fmt::format("You have died!"));
    g_state = std::make_unique<state::Dead>();
  } else {
    if (world.active_map().visible.at(target.pos)) {
      world.log.append(fmt::format("The {} dies!", target.name));
    }
    world.actors.erase(target.id);
  }
}

inline auto attack(World& world, Actor& self, Actor& target) {
  auto damage = self.stats.attack;
  damage -= target.stats.defense;
  target.stats.hp -= damage;
  damage = std::max(0, damage);
  const auto& map = world.active_map();
  if (map.visible.at(self.pos) || map.visible.at(target.pos)) {
    if (damage > 0) {
      world.log.append(fmt::format("{} attacks {} for {} hit points.", self.name, target.name, damage));
    } else {
      world.log.append(fmt::format("{} attacks {} but it has no effect!", self.name, target.name));
    }
  }
  if (target.stats.hp <= 0) {
    kill(world, target);
  };
}

inline auto heal(World& world, Actor& target, int amount) {
  amount = std::min(amount, target.stats.max_hp - target.stats.hp);
  target.stats.hp += amount;
  const auto& map = world.active_map();
  if (map.visible.at(target.pos)) {
    world.log.append(fmt::format("{} heals {} HP.", target.name, amount));
  }
  return amount;
}

}  // namespace combat
