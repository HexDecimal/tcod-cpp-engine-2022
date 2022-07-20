#pragma once
#include <cassert>

#include "../maptools.hpp"
#include "../types/ndarray.hpp"
#include "../types/position.hpp"
#include "pathfinding.hpp"

namespace pf {
using Index2 = Position;  // 2D coordinates.

[[nodiscard]] inline auto setup_graph(const util::Array2D<int>& cost, int cardinal = 2, int diagonal = 3) {
  return [=, &cost](const Index2& xy, auto add_edge) {
    const auto check_add_edge = [&](int x, int y, int edge_cost) {
      if (!cost.in_bounds({x, y})) return;
      edge_cost *= cost.at({x, y});
      if (edge_cost <= 0) return;
      add_edge({x, y}, edge_cost);
    };
    for (const auto adj_xy : std::array<Index2, 4>{Index2{0, -1}, Index2{-1, 0}, Index2{1, 0}, Index2{0, 1}}) {
      check_add_edge(xy.x + adj_xy.x, xy.y + adj_xy.y, cardinal);
    };
    for (const auto adj_xy : std::array<Index2, 4>{Index2{-1, -1}, Index2{1, -1}, Index2{-1, 1}, Index2{1, 1}}) {
      check_add_edge(xy.x + adj_xy.x, xy.y + adj_xy.y, diagonal);
    };
  };
}

[[nodiscard]] inline auto setup_set_edge(util::Array2D<int>& dist) {
  return [&](Index2 dest, Index2 origin, int edge_distance) -> bool {
    const auto next_dist = dist.at(origin) + edge_distance;
    if (dist.at(dest) <= next_dist) return false;
    dist.at(dest) = next_dist;
    return true;
  };
}
[[nodiscard]] inline auto setup_set_edge(util::Array2D<int>& dist, util::Array2D<Index2>& flow) {
  return [&](Index2 dest, Index2 origin, int edge_distance) -> bool {
    const auto next_dist = dist.at(origin) + edge_distance;
    if (dist.at(dest) <= next_dist) return false;
    dist.at(dest) = next_dist;
    flow.at(dest) = origin;
    return true;
  };
}

[[nodiscard]] inline auto new_flow_array(std::array<int, 2> shape) -> util::Array2D<Index2> {
  auto flow = util::Array2D<Index2>(shape);
  with_indexes(flow, [&](int x, int y) { flow.at({x, y}) = {x, y}; });
  return flow;
}

/// Return a path along a flow-map from start to a neutral index.
[[nodiscard]] inline auto get_path(const util::Array2D<Index2>& flow, Index2 start) -> std::vector<Index2> {
  auto path = std::vector<Index2>{};
  path.emplace_back(start);
  while (path.back() != flow.at(path.back())) {
    assert(std::find(path.begin(), path.end(), flow.at(path.back())) == path.end());  // Recursion check.
    path.emplace_back(flow.at(path.back()));
  }
  return path;
}

}  // namespace pf
