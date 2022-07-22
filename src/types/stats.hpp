#pragma once
#include <vector>

#include "item.hpp"

struct Stats {
  int max_hp;
  int hp;
  int attack;
  int defense;
  std::vector<Item> inventory;
};
