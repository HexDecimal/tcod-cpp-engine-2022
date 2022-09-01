#pragma once

#include <fmt/core.h>

#include "../combat.hpp"
#include "../distance.hpp"
#include "../item_tools.hpp"
#include "../states/pick_tile_aoe.hpp"
#include "../types/actor.hpp"
#include "../types/item.hpp"
#include "../types/world.hpp"
#include "../world_logic.hpp"

struct FireballScroll : public Item {
  [[nodiscard]] virtual std::string get_name() const override { return "scroll of fireball"; }
  [[nodiscard]] virtual std::tuple<int, tcod::ColorRGB> get_graphic() const override {
    return {'#', tcod::ColorRGB{255, 63, 63}};
  }
  [[nodiscard]] virtual action::Result use_item(World& world, Actor& actor) override {
    auto on_pick = [&](Position target_pos) {
      auto target_ids = std::vector<ActorID>{};
      for (auto& target_id : world.active_actors) {
        if (euclidean_squared(world.get(target_id).pos - target_pos) < range_squared)
          target_ids.emplace_back(target_id);
      }
      for (auto target_id : target_ids) {
        auto& target = world.get(target_id);
        int damage = combat::calculate_damage(world, target, atk_damage);
        world.log.append(fmt::format("The {} gets burned for {} hit points.", target.name, damage));
        combat::apply_damage(world, target, damage);
      }
      consume_discard_item(actor, this);
      return state::EndTurn{};
    };
    const auto& map = world.active_map();
    const auto is_not_self_and_visible = [&](const Actor& other) {
      return other.id != actor.id && map.visible.at(other.pos);
    };
    const auto* nearest_visible_enemy = get_nearest_actor(world, actor.pos, is_not_self_and_visible);
    g_controller.cursor = nearest_visible_enemy ? nearest_visible_enemy->pos : actor.pos;
    return action::Poll{std::make_unique<state::PickTileAreaOfEffect>(std::move(g_state), on_pick, range_squared)};
  };
  int range_squared = static_cast<int>(2.5 * 2.5) + 1;
  int atk_damage = 12;
};
