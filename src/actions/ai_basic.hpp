#pragma once
#include "../distance.hpp"
#include "../pathfinding/astar.hpp"
#include "../types/position.hpp"
#include "ai_confused.hpp"
#include "base.hpp"
#include "bump.hpp"

namespace action {
class BasicAI : public Action {
 public:
  [[nodiscard]] virtual Result perform(World& world, Actor& actor) override {
    if (actor.stats.confused_turns) {
      --actor.stats.confused_turns;
      return ConfusedAI{}.perform(world, actor);
    }
    const Map& map = world.active_map();
    const auto& player = world.active_player();
    const auto can_see_player = map.visible.at(actor.pos);
    if (can_see_player) {
      auto cost = util::Array2D<int>{map.get_size()};
      with_indexes(map, [&](int x, int y) { return cost.at({x, y}) = map.tiles.at({x, y}) == Tiles::wall ? 0 : 1; });
      for (auto actor_id : world.active_actors) cost.at(world.get(actor_id).pos) += 10;
      cost.at(player.pos) = 1;
      path_ = pf::get_astar2d_path(cost, actor.pos, player.pos);
    }
    if (path_.size() && path_.back() == actor.pos) path_.pop_back();
    if (path_.size()) {
      const auto move_dir = path_.back() - actor.pos;
      if (chebyshev(move_dir) > 1) {
        path_.clear();
        return Success{};
      }
      return Bump(move_dir).perform(world, actor);
    }
    return Success{};
  };

 private:
  std::vector<Position> path_;
  static auto sign_(int n) -> int { return (n == 0 ? 0 : (n < 0 ? -1 : 1)); }
};

}  // namespace action
