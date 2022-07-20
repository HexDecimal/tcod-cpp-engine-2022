#pragma once
#include <algorithm>
#include <tuple>
#include <vector>
namespace pf {
/// A generic pathfinder template.
template <typename IndexType, typename DistType = int>
class Pathfinder {
 public:
  Pathfinder() = default;

  /// Add an index to this frontier.
  template <typename Heuristic>
  void add(const IndexType& index, const Heuristic& heuristic) {
    frontier_.push_back(NodeType{heuristic(index), index});
    std::push_heap(frontier_.begin(), frontier_.end(), get_frontier_predicate_());
  }

  /// Clear all indexes from this frontier.
  void clear() noexcept { frontier_.clear(); };

  /// Change the frontier heap to use a different heuristic.
  template <typename Heuristic>
  void change_heuristic(const Heuristic& heuristic) {
    for (auto& it : frontier_) it.distance = heuristic(it.index);
    std::make_heap(frontier_.begin(), frontier_.end(), get_frontier_predicate_());
  }

  /// Run a pathfinder until a goal is reached.
  template <typename GraphFunc, typename Heuristic, typename SetEdgeFunc, typename GoalFunc>
  void compute(
      const GraphFunc& graph,  // [](const IndexType& index, add_edge) -> void { add_edge(dest, distance); }
      const Heuristic& heuristic,  // [](const IndexType& index) -> DistType {}
      const SetEdgeFunc& set_edge,  // [](dest, origin, distance) -> bool {}
      const GoalFunc& is_goal = [](const IndexType&) -> bool { return false; }) {
    while (frontier_.size()) {
      const IndexType current_index = frontier_.at(0).index;
      if (is_goal(current_index)) return;
      const auto add_edge = [&](const IndexType& next_index, const DistType& distance) {
        if (!set_edge(next_index, current_index, distance)) {
          return;  // set_edge should return false if the edge would go backwards.
        }
        frontier_.push_back(NodeType{heuristic(next_index), next_index});
        std::push_heap(frontier_.begin(), frontier_.end(), get_frontier_predicate_());
      };
      std::pop_heap(frontier_.begin(), frontier_.end(), get_frontier_predicate_());
      frontier_.pop_back();
      graph(current_index, add_edge);
    }
  }

 private:
  struct NodeType {
    DistType distance;  // Distance plus heuristic.
    IndexType index;  // Node index.
  };
  constexpr auto get_frontier_predicate_() {
    return [](const NodeType& lhs, const NodeType& rhs) -> bool { return lhs.distance > rhs.distance; };
  }
  std::vector<NodeType> frontier_;  // The frontier heap queue.
};
}  // namespace pf
