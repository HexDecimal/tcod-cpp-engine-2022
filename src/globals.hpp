#pragma once
#include <libtcod.hpp>

#include "state.hpp"

inline tcod::Console g_console;  // The global console object.
inline tcod::Context g_context;  // The global libtcod context.
inline std::array<int, 2> player_xy{40, 25};  // Player position.
inline std::unique_ptr<state::State> g_state;
