#pragma once
#include <libtcod.hpp>

#include "state.hpp"
#include "world.hpp"

inline tcod::Console g_console;  // The global console object.
inline tcod::Context g_context;  // The global libtcod context.
inline std::unique_ptr<state::State> g_state;
inline std::unique_ptr<World> g_world;  // The active world object.
