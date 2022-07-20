#pragma once
#include <cassert>

#include "../types/ndarray.hpp"
#include "map.hpp"

namespace pf {
[[nodiscard]] inline auto setup_heuristic(const util::Array2D<int>& dist) {
  return [&](Index2 xy) { return dist.at(xy); };
}

[[nodiscard]] inline auto dijkstra2d(
    util::Array2D<int>& dist, const util::Array2D<int>& cost, int cardinal = 2, int diagonal = 3) -> void {
  assert(dist.get_shape() == cost.get_shape());
  auto pathfinder = pf::Pathfinder<Index2>{};
  const auto heuristic = setup_heuristic(dist);
  with_indexes(dist, [&](int x, int y) {
    if (dist.at({x, y}) == std::numeric_limits<int>::max()) return;
    pathfinder.add({x, y}, heuristic);
  });
  const auto is_goal = [](auto) { return false; };
  pathfinder.compute(setup_graph(cost, cardinal, diagonal), heuristic, setup_set_edge(dist), is_goal);
}

[[nodiscard]] inline auto dijkstra2d(
    Index2& start_xy, const util::Array2D<int>& cost, int cardinal = 2, int diagonal = 3) -> util::Array2D<int> {
  auto dist = util::Array2D<int>{cost.get_shape(), std::numeric_limits<int>::max()};
  dist.at(start_xy) = 0;
  auto pathfinder = pf::Pathfinder<Index2>{};
  const auto heuristic = setup_heuristic(dist);
  pathfinder.add(start_xy, heuristic);
  const auto is_goal = [](auto) { return false; };
  pathfinder.compute(setup_graph(cost, cardinal, diagonal), heuristic, setup_set_edge(dist), is_goal);
  return dist;
}
}  // namespace pf
