#pragma once

#include <optional>

#include "position.hpp"

struct Controller {
  std::optional<Position> cursor = {};
};
