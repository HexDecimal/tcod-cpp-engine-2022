#pragma once
#include <algorithm>
#include <cmath>

#include "types/position.hpp"

/// Return the euclidean distance of a vector.
template <typename T>
inline T euclidean(T x, T y) {
  return static_cast<T>(std::sqrt(x * x + y * y));
}
inline int euclidean(Position vec) { return euclidean(vec.x, vec.y); }

/// Return the squared euclidean distance of a vector.
template <typename T>
inline T euclidean_squared(T x, T y) {
  return x * x + y * y;
}
inline int euclidean_squared(Position vec) { return euclidean_squared(vec.x, vec.y); }

/// Return the rectilinear distance of a vector.
template <typename T>
inline T manhattan(T x, T y) {
  return std::abs(x) + std::abs(y);
}
inline int manhattan(Position vec) { return manhattan(vec.x, vec.y); }

/// Return the maximum metric, chessboard kings-moves of a vector.
template <typename T>
inline T chebyshev(T x, T y) {
  return std::max(std::abs(x), std::abs(y));
}
inline int chebyshev(Position vec) { return chebyshev(vec.x, vec.y); }
