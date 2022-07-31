#pragma once
#include <fmt/core.h>

#include "base.hpp"

namespace action {
class Pickup : public Action {
 public:
  [[nodiscard]] virtual Result perform(World& world, Actor& actor) override {
    Map& map = world.active_map();
    auto range = map.items.equal_range(actor.pos);
    if (range.first == map.items.end()) {
      return Failure{"Nothing to pickup!"};
    }
    auto& item = range.first->second;
    auto stack_item =
        std::find_if(actor.stats.inventory.begin(), actor.stats.inventory.end(), [&](auto& inventory_item) {
          return inventory_item->get_name() == item->get_name();
        });
    // Take or stack the item.
    world.log.append(fmt::format("You take the {}.", item->get_name()));
    if (stack_item != actor.stats.inventory.end()) {
      (*stack_item)->count += item->count;
    } else {
      actor.stats.inventory.emplace_back(std::move(item));
    }
    map.items.erase(range.first);
    return Success{};
  };

 private:
};

}  // namespace action
