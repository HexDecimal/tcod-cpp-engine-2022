#pragma once
#include <fmt/core.h>

#include <cassert>
#include <gsl/gsl>

#include "../combat.hpp"
#include "../states/pick_tile.hpp"
#include "base.hpp"

namespace action {
class UseItem : public Action {
 public:
  UseItem() = default;
  UseItem(int item_index) : item_index_{item_index} { assert(item_index_ >= 0); }
  [[nodiscard]] virtual Result perform(World& world, Actor& actor) override {
    if (item_index_ >= gsl::narrow<decltype(item_index_)>(actor.stats.inventory.size())) {
      return Failure{"You don't have that item."};
    }
    auto& item = actor.stats.inventory.at(item_index_);
    /*
    if (item.name == "health potion") {
      world.log.append(fmt::format("You use the {}.", item.name));
      combat::heal(world, actor, 4);
      --item.count;
      if (item.count <= 0) {
        actor.stats.inventory.erase(actor.stats.inventory.begin() + item_index_);
      }
    } else if (item.name == "scroll of lightning bolt") {
      g_controller.cursor = g_world->active_player().pos;
      return Poll{std::make_unique<state::PickTile>(
          std::move(g_state), [this_actor_id = actor.id, &item = item](auto target_pos) -> state::Result {
            auto affected = std::vector<ActorID>{};
            for (auto& [actor_id, actor] : g_world->actors) {
              if (actor.pos == target_pos) affected.emplace_back(actor_id);
            }
            for (const auto actor_id : affected) {
              combat::kill(*g_world, g_world->actors.at(actor_id));
            }
            g_controller.cursor = std::nullopt;
            consume_item(this_actor_id, item);
            return state::EndTurn{};
          })};
    } else {
      assert(0);
    }
    */
    return item->use_item(world, actor);
  };

 private:
  int item_index_ = 0;
};

}  // namespace action
