#pragma once

#include <fmt/core.h>

#include "../types/actor.hpp"
#include "../types/item.hpp"
#include "../types/world.hpp"

struct HealthPotion : public Item {
  [[nodiscard]] virtual std::string get_name() const override { return "health potion"; }
  [[nodiscard]] virtual std::tuple<int, tcod::ColorRGB> get_graphic() const override { return {'!', {128, 21, 21}}; }
  [[nodiscard]] virtual action::Result use_item(World& world, Actor& actor) {
    world.log.append(fmt::format("You use the {}.", get_name()));
    combat::heal(world, actor, 4);
    --count;
    if (count <= 0) {
      auto& inventory = actor.stats.inventory;
      inventory.erase(std::find_if(inventory.begin(), inventory.end(), [&](auto& it) { return it.get() == this; }));
    }
    return action::Success{};
  };
};
