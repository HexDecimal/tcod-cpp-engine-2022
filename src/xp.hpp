#pragma once

inline auto next_level_xp(int level) -> int {
  const int LEVEL_UP_BASE = 200;
  const int LEVEL_UP_FACTOR = 150;
  return LEVEL_UP_BASE + level * LEVEL_UP_FACTOR;
}
