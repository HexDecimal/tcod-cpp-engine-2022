#pragma once
#include <array>

struct Position {
  // Allow this struct to be used in subscript operators as {x, y}.
  operator const std::array<int, 2>() const noexcept { return {x, y}; }
  int x;
  int y;
};
