#pragma once
#include "main_menu.hpp"

#include <memory>

#include "../globals.hpp"
#include "../world_init.hpp"
#include "ingame.hpp"
#include "menu.hpp"

namespace state {
MainMenu::MainMenu(int selected)
    : Menu{
          MenuItems{
              {"New Game",
               []() -> state::Result {
                 g_world = new_world();
                 return state::Change{std::make_unique<state::InGame>()};
               }},
              {"Continue",
               []() -> state::Result {
                 if (g_world) return state::Change{std::make_unique<state::InGame>()};
                 return {};
               }},
              {"Exit", []() -> state::Result { std::exit(EXIT_SUCCESS); }},
          },
          selected} {}
}  // namespace state
