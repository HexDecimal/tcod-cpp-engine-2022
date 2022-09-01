#pragma once

#include <fmt/core.h>

#include "../combat.hpp"
#include "../item_tools.hpp"
#include "../types/actor.hpp"
#include "../types/item.hpp"
#include "../types/world.hpp"

struct HealthPotion : public Item {
  [[nodiscard]] virtual std::string get_name() const override { return "health potion"; }
  [[nodiscard]] virtual std::tuple<int, tcod::ColorRGB> get_graphic() const override { return {'!', {128, 21, 21}}; }
  [[nodiscard]] virtual action::Result use_item(World& world, Actor& actor) override {
    world.log.append(fmt::format("You drink the {}.", get_name()));
    combat::heal(world, actor, 4);
    consume_discard_item(actor, this);
    return action::Success{};
  };
};
