#pragma once
#include <vector>

#include "item.hpp"

struct Stats {
  int max_hp;
  int hp;
  int attack;
  int defense;
  std::vector<std::unique_ptr<Item>> inventory;
  int confused_turns = 0;
};
