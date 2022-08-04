#pragma once
#include <memory>
#include <variant>

#include "state_fwd.hpp"
namespace state {
struct Change {
  std::unique_ptr<state::State> new_state;
};
struct Reset {};
struct EndTurn {};
struct Quit {};

using Result = std::variant<std::monostate, Change, Reset, EndTurn, Quit>;
}  // namespace state
