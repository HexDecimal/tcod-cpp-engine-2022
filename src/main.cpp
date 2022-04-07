
#include <SDL.h>
#include <libtcod.h>

#include <cstdlib>

int main(int argc, char** argv) {
  auto params = TCOD_ContextParams{};
  params.tcod_version = TCOD_COMPILEDVERSION;
  params.argc = argc;
  params.argv = argv;
  params.renderer_type = TCOD_RENDERER_SDL2;
  params.vsync = 1;
  params.sdl_window_flags = SDL_WINDOW_RESIZABLE;
  params.window_title = "Libtcod Template Project";

  auto tileset = tcod::load_tilesheet("data/dejavu16x16_gs_tc.png", {32, 8}, tcod::CHARMAP_TCOD);
  params.tileset = tileset.get();

  auto console = tcod::Console{80, 40};
  params.console = console.get();

  auto context = tcod::new_context(params);

  // Game loop.
  while (true) {
    // Rendering.
    console.clear();
    tcod::print(console, {0, 0}, "Hello World", TCOD_white, std::nullopt);
    context->present(console);

    // Handle input.
    SDL_Event event;
    SDL_WaitEvent(nullptr);
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT:
          std::exit(EXIT_SUCCESS);
          break;
      }
    }
  }
}
