#pragma once
#include <string>
#include <variant>
namespace action {
struct Success {};
struct Failure {
  std::string reason;
};
using Result = std::variant<Success, Failure>;
}  // namespace action
