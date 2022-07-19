#pragma once
/// Call func the border indexes on an array of the given shape.
template <typename Func>
inline void with_border(int width, int height, const Func& func) {
  for (int y{0}; y < height; ++y) {
    for (int x{0}; x < width; ++x) {
      if (x == 0 || y == 0 || x == width - 1 || y == height - 1) func(x, y);
    }
  }
}
/// Call func the border indexes of the given array.
template <typename Array, typename F>
inline void with_border(const Array& array, F func) {
  with_border(array.get_width(), array.get_height(), func);
}
/// Call func with all the indexes in an array of the given shape.
template <typename Func>
inline void with_indexes(int width, int height, Func func) {
  for (int y{0}; y < height; ++y) {
    for (int x{0}; x < width; ++x) {
      func(x, y);
    }
  }
}
/// Call func with all the indexes in an array.
template <typename Array, typename F>
inline void with_indexes(const Array& array, F func) {
  with_indexes(array.get_width(), array.get_height(), func);
}
