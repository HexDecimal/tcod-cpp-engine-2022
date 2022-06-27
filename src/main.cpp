#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif  // __EMSCRIPTEN__
#include <SDL.h>

#include <cstdlib>
#include <iostream>
#include <libtcod.hpp>

#include "data.hpp"

#if defined(_MSC_VER)
#pragma warning(disable : 4297)  // Allow "throw" in main().  Letting the compiler handle termination.
#endif

static tcod::Console g_console;  // The global console object.
static tcod::Context g_context;  // The global libtcod context.
static std::array<int, 2> player_xy{40, 25};  // Player position.

/// Game loop.
static void main_loop() {
  // Rendering.
  g_console.clear();
  if (g_console.in_bounds(player_xy)) {
    g_console.at(player_xy) = {'@', tcod::ColorRGB{255, 255, 255}, tcod::ColorRGB{0, 0, 0}};
  }
  g_context.present(g_console);

  // Handle input.
  SDL_Event event;
#ifndef __EMSCRIPTEN__
  // Block until events exist.  This conserves resources well but isn't compatible with animations or Emscripten.
  SDL_WaitEvent(nullptr);
#endif
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_KEYDOWN:
        switch (event.key.keysym.sym) {
          case SDLK_UP:
            player_xy.at(1) -= 1;
            break;
          case SDLK_DOWN:
            player_xy.at(1) += 1;
            break;
          case SDLK_LEFT:
            player_xy.at(0) -= 1;
            break;
          case SDLK_RIGHT:
            player_xy.at(0) += 1;
            break;
          default:
            break;
        }
        break;
      case SDL_QUIT:
        std::exit(EXIT_SUCCESS);
        break;
    }
  }
}

/// Main program entry point.
int main(int argc, char** argv) {
  try {
    auto params = TCOD_ContextParams{};
    params.tcod_version = TCOD_COMPILEDVERSION;
    params.argc = argc;
    params.argv = argv;
    params.renderer_type = TCOD_RENDERER_SDL2;
    params.vsync = 1;
    params.sdl_window_flags = SDL_WINDOW_RESIZABLE;
    params.window_title = "Libtcod Template Project";

    auto tileset = tcod::load_tilesheet(get_data_dir() / "dejavu16x16_gs_tc.png", {32, 8}, tcod::CHARMAP_TCOD);
    params.tileset = tileset.get();

    g_console = tcod::Console{80, 50};
    params.console = g_console.get();

    g_context = tcod::Context(params);

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
