
#include <SDL.h>

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <libtcod.hpp>

#if defined(_MSC_VER)
#pragma warning(disable : 4297)  // Allow "throw" in main().  Letting the compiler handle termination.
#endif

/// Return the data directory.
auto get_data_dir() -> std::filesystem::path {
  static auto root_directory = std::filesystem::path{"."};  // Begin at the working directory.
  while (!std::filesystem::exists(root_directory / "data")) {
    // If the current working directory is missing the data dir then it will assume it exists in any parent directory.
    root_directory /= "..";
    if (!std::filesystem::exists(root_directory)) {
      throw std::runtime_error("Could not find the data directory.");
    }
  }
  return root_directory / "data";
};

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
  } catch (const std::exception& exc) {
    std::cerr << exc.what() << "\n";
    throw;
  }
}
