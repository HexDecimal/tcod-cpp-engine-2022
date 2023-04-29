#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#endif  // __EMSCRIPTEN__
#include <SDL.h>

#include <cassert>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <libtcod.hpp>

#include "constants.hpp"
#include "data.hpp"
#include "globals.hpp"
#include "procgen/caves.hpp"
#include "rendering.hpp"
#include "serialization.hpp"
#include "states/ingame.hpp"
#include "states/levelup.hpp"
#include "states/main_menu.hpp"
#include "world_init.hpp"

#ifndef EMSCRIPTEN_KEEPALIVE
#define EMSCRIPTEN_KEEPALIVE
#endif

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
      auto result = g_state->on_event(event);
      if (std::holds_alternative<std::monostate>(result)) {
      } else if (std::holds_alternative<state::Change>(result)) {
        g_state = std::move(std::get<state::Change>(result).new_state);
      } else if (std::holds_alternative<state::Reset>(result)) {
        g_controller.cursor = std::nullopt;
        g_state = std::make_unique<state::InGame>();
      } else if (std::holds_alternative<state::EndTurn>(result)) {
        g_controller.cursor = std::nullopt;
        if (g_world->active_player().stats.hp > 0) {
          g_state = std::make_unique<state::InGame>();
          update_fov(g_world->active_map(), g_world->active_player().pos);
          enemy_turn(*g_world);
          if (g_world->active_player().stats.xp >= next_level_xp(g_world->active_player().stats.level)) {
            g_state = std::make_unique<state::LevelUp>();
          }
        } else {
          g_state = std::make_unique<state::Dead>();
        }
      } else if (std::holds_alternative<state::Quit>(result)) {
        if (g_world) save_world(*g_world);
        std::exit(EXIT_SUCCESS);
      } else {
        assert(0);
      }
    }
  }
}

extern "C" void EMSCRIPTEN_KEEPALIVE after_main_init() {
  g_world = load_world();
  g_state = std::make_unique<state::MainMenu>();
}

extern "C" const char* EMSCRIPTEN_KEEPALIVE on_page_unload(int, const void*, void*) {
  if (g_world) save_world(*g_world);
  return nullptr;
}

// Main initialization code.
void main_init(int argc = 0, char** argv = nullptr) {
#ifndef __EMSCRIPTEN__
  std::atexit(SDL_Quit);
#endif
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

  g_console = tcod::Console{constants::CONSOLE_WIDTH, constants::CONSOLE_HEIGHT};
  params.console = g_console.get();

  g_context = tcod::Context(params);

#ifdef __EMSCRIPTEN__
  // clang-format off
  EM_ASM(
    FS.mkdir('/saves');
    FS.mount(IDBFS, {}, '/saves');
    FS.syncfs(true, function (err) {
      assert(!err);
      ccall('after_main_init', null);
    });
  );
  // clang-format on
#else
  after_main_init();
#endif
}

/// Main program entry point.
int main(int argc, char** argv) {
  try {
    main_init(argc, argv);
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(main_loop, 0, 0);
    emscripten_set_beforeunload_callback(nullptr, on_page_unload);
#else
    while (true) main_loop();
#endif
  } catch (const std::exception& exc) {
    std::cerr << exc.what() << "\n";
    throw;
  }
}
