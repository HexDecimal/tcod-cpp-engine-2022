#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif  // __EMSCRIPTEN__
#include <SDL.h>

#include <cstdlib>
#include <iostream>
#include <libtcod.hpp>

#include "data.hpp"
#include "globals.hpp"
#include "mapgen.hpp"
#include "rendering.hpp"
#include "states/ingame.hpp"

#if defined(_MSC_VER)
#pragma warning(disable : 4297)  // Allow "throw" in main().  Letting the compiler handle termination.
#endif

/// Game loop.
static void main_loop() {
  // Rendering.
  main_redraw();

  // Handle input.
  SDL_Event event;
#ifndef __EMSCRIPTEN__
  // Block until events exist.  This conserves resources well but isn't compatible with animations or Emscripten.
  SDL_WaitEvent(nullptr);
#endif
  while (SDL_PollEvent(&event)) {
    if (g_state) {
      if (auto new_state = g_state->on_event(event); new_state) {
        g_state = std::move(new_state);
      }
    }
  }
}

// Main initialization code.
void main_init(int argc = 0, char** argv = nullptr) {
  auto params = TCOD_ContextParams{};
  params.tcod_version = TCOD_COMPILEDVERSION;
  params.argc = argc;
  params.argv = argv;
  params.renderer_type = TCOD_RENDERER_SDL2;
  params.vsync = 1;
  params.sdl_window_flags = SDL_WINDOW_RESIZABLE;
  params.window_title = "Libtcod Engine 2022";

  auto tileset = tcod::load_tilesheet(get_data_dir() / "dejavu16x16_gs_tc.png", {32, 8}, tcod::CHARMAP_TCOD);
  params.tileset = tileset.get();

  g_console = tcod::Console{80, 50};
  params.console = g_console.get();

  g_context = tcod::Context(params);

  g_state = std::make_unique<state::InGame>();
  g_world = std::make_unique<World>();
  auto& player = g_world->actors.emplace(0, Actor{}).first->second;
  player.name = "player";
  player.ch = '@';
  player.fg = {255, 255, 255};
  player.stats.max_hp = player.stats.hp = 30;
  player.stats.attack = 5;
  player.stats.defense = 2;

  g_world->schedule.emplace_back(0);

  procgen::generate_level(*g_world);
}

/// Main program entry point.
int main(int argc, char** argv) {
  try {
    main_init(argc, argv);
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(main_loop, 0, 0);
#else
    while (true) main_loop();
#endif
  } catch (const std::exception& exc) {
    std::cerr << exc.what() << "\n";
    throw;
  }
}
