#pragma once
#include <fmt/core.h>

#include "../combat.hpp"
#include "base.hpp"

namespace action {
class UseItem : public Action {
 public:
  UseItem() = default;
  UseItem(int item_index) : item_index_{item_index} { assert(item_index_ >= 0); }
  [[nodiscard]] virtual Result perform(World& world, Actor& actor) override {
    if (item_index_ >= actor.stats.inventory.size()) {
      return Failure{"You don't have that item."};
    }
    auto& item = actor.stats.inventory.at(item_index_);
    world.log.append(fmt::format("You use the {}.", item.name));
    combat::heal(world, actor, 4);
    --item.count;
    if (item.count <= 0) {
      actor.stats.inventory.erase(actor.stats.inventory.begin() + item_index_);
    }
    return Success{};
  };

 private:
  int item_index_ = 0;
};

}  // namespace action
