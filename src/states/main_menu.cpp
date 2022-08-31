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
              {"[N] New Game",
               []() -> state::Result {
                 g_world = new_world();
                 return state::Change{std::make_unique<state::InGame>()};
               },
               SDLK_n},
              {"[C] Continue",
               []() -> state::Result {
                 if (g_world) return state::Change{std::make_unique<state::InGame>()};
                 return {};
               },
               SDLK_c},
#ifndef __EMSCRIPTEN__
              {"[Q] Exit", []() -> state::Result { return Quit{}; }, SDLK_q},
#endif  // __EMSCRIPTEN__
          },
          selected} {
}
}  // namespace state
