#pragma once

#include <fmt/core.h>

#include "../combat.hpp"
#include "../distance.hpp"
#include "../item_tools.hpp"
#include "../types/actor.hpp"
#include "../types/item.hpp"
#include "../types/world.hpp"
#include "../world_logic.hpp"

struct LightningScroll : public Item {
  [[nodiscard]] virtual std::string get_name() const override { return "scroll of lightning bolt"; }
  [[nodiscard]] virtual std::tuple<int, tcod::ColorRGB> get_graphic() const override {
    return {'#', tcod::ColorRGB{255, 255, 63}};
  }
  [[nodiscard]] virtual action::Result use_item(World& world, Actor& actor) override {
    Map& map = world.active_map();
    const auto is_not_player_and_visible = [&](const Actor& other) {
      return other.id != actor.id && map.visible.at(other.pos);
    };
    Actor* target = get_nearest_actor(world, actor.pos, is_not_player_and_visible, range_squared);
    if (!target) return action::Failure{"No enemy is close enough to strike."};

    const int damage = combat::calculate_damage(world, *target, atk_damage);
    world.log.append(fmt::format(
        "A lighting bolt strikes the {} with a loud thunder!\n"
        "The damage is {} hit points.",
        target->name,
        damage));
    combat::apply_damage(world, *target, damage);
    consume_discard_item(actor, this);
    return action::Success{};
  };
  int range_squared = static_cast<int>(5.5 * 5.5) + 1;
  int atk_damage = 20;
};
