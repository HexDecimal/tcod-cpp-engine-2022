#pragma once
#include <string>
#include <variant>

#include "state.hpp"
namespace action {
struct Success {};
struct Failure {
  std::string reason;
};
struct Poll {
  std::unique_ptr<state::State> new_state;
};
using Result = std::variant<Success, Failure, Poll>;
}  // namespace action
