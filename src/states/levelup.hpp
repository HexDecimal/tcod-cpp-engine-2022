#pragma once
#include <fmt/core.h>

#include "../globals.hpp"
#include "../world_init.hpp"
#include "../xp.hpp"
#include "menu.hpp"

namespace state {
class LevelUp : public Menu {
 public:
  explicit LevelUp(int selected = 0)
      : Menu{
            MenuItems{
                {"Constitution (+20HP)",
                 []() -> StateReturnType {
                   g_world->active_player().stats.hp += 20;
                   g_world->active_player().stats.max_hp += 20;
                   return level_up_done();
                 }},
                {"Strength (+1 attack)",
                 []() -> StateReturnType {
                   g_world->active_player().stats.attack += 1;
                   return level_up_done();
                 }},
                {"Agility (+1 defense)",
                 []() -> StateReturnType {
                   g_world->active_player().stats.defense += 1;
                   return level_up_done();
                 }}},
            selected} {
    assert(g_world);
    g_world->log.append(fmt::format(
        "Your battle skills grow stronger! You reached level {}.", g_world->active_player().stats.level + 1));
  }

 private:
  static auto level_up_done() -> StateReturnType {
    auto& player = g_world->active_player();
    player.stats.xp -= next_level_xp(player.stats.level);
    ++player.stats.level;
    return Reset{};
  }
};
}  // namespace state
