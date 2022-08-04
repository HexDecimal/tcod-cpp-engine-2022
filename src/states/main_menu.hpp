#pragma once
#include <cassert>

#include "menu.hpp"

namespace state {
class MainMenu : public Menu {
 public:
  explicit MainMenu(int selected = 0);
};
}  // namespace state
