#pragma once

#include "types/map.hpp"
#include "types/world.hpp"

auto generate_level(World& world) -> Map& {
  auto& map = world.maps["main"] = Map{80, 45};
  return map;
}
