#pragma once
#include <functional>
#include <string>

struct MapID {
  bool operator==(const MapID rhs) const noexcept { return this->name == rhs.name && this->level == rhs.level; }
  bool operator!=(const MapID rhs) const noexcept { return !(*this == rhs); }

  std::string name;
  int level = 0;
};

template <>
struct std::hash<MapID> {
  std::size_t operator()(const MapID& id) const {
    return std::hash<decltype(id.name)>{}(id.name) ^ std::hash<decltype(id.level)>{}(id.level);
  }
};
