#pragma once
#include <array>

struct Position {
  // Allow this struct to be used in subscript operators as {x, y}.
  operator const std::array<int, 2>() const noexcept { return {x, y}; }
  Position operator+(const Position rhs) const noexcept { return {this->x + rhs.x, this->y + rhs.y}; }
  Position operator-(const Position rhs) const noexcept { return {this->x - rhs.x, this->y - rhs.y}; }
  bool operator==(const Position rhs) const noexcept { return this->x == rhs.x && this->y == rhs.y; }
  bool operator!=(const Position rhs) const noexcept { return !(*this == rhs); }

  int x;
  int y;
};
