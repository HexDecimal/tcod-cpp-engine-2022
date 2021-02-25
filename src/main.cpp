
#include <SDL.h>
#include <libtcod.h>

#include <cstdlib>

int main(int argc, char** argv) {
  TCOD_ContextParams params = {TCOD_COMPILEDVERSION};
  params.argc = argc;
  params.argv = argv;
  params.vsync = true;
  params.sdl_window_flags = SDL_WINDOW_RESIZABLE;

  tcod::ConsolePtr console = tcod::new_console(80, 25);
  params.columns = console->w;
  params.rows = console->h;

  tcod::ContextPtr context = tcod::new_context(params);

  // Game loop.
  while (true) {
    // Rendering.
    TCOD_console_clear(console.get());
    tcod::print(*console, 0, 0, "Hello World", &TCOD_white, nullptr, TCOD_BKGND_SET, TCOD_LEFT);
    context->present(*console);

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
  return 0;
}
