#pragma once
#include <algorithm>
#include <cassert>

#include "../types/ndarray.hpp"
#include "map.hpp"

namespace pf {
[[nodiscard]] inline auto setup_heuristic(
    const util::Array2D<int>& dist, const Index2 goal, int cardinal = 2, int diagonal = 3) {
  return [=, &dist](Index2 pos) {
    const int diff_x = std::abs(pos.x - goal.x);
    const int diff_y = std::abs(pos.y - goal.y);
    const int cardinal_len = std::min(diff_x, diff_y);
    const int diagonal_len = std::max(diff_x, diff_y) - cardinal_len;
    return dist.at(pos) + cardinal_len * cardinal + diagonal_len * diagonal;
  };
}

/// Return the path from root to goal using A*.
/// The path returned begins at goal and ends at the root.
[[nodiscard]] inline auto get_astar2d_path(
    const util::Array2D<int>& cost, Index2 root, Index2 goal, int cardinal = 2, int diagonal = 3)
    -> std::vector<Index2> {
  auto flow = new_flow_array(cost.get_shape());
  auto dist = util::Array2D<int>{cost.get_shape(), std::numeric_limits<int>::max()};
  dist.at(root) = 0;
  auto pathfinder = pf::Pathfinder<Index2>{};
  const auto heuristic = setup_heuristic(dist, goal, cardinal, diagonal);
  pathfinder.add(root, heuristic);
  const auto is_goal = [&](Index2 pos) { return pos == goal; };
  pathfinder.compute(setup_graph(cost, cardinal, diagonal), heuristic, setup_set_edge(dist, flow), is_goal);
  return get_path(flow, goal);
}
}  // namespace pf
