#pragma once
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif  // __EMSCRIPTEN__

#include <filesystem>
#include <fstream>
#include <iostream>
#include <libtcod/color.hpp>
#include <sstream>

#include "actions/ai_basic.hpp"
#include "items/health_potion.hpp"
#include "items/scroll_confusion.hpp"
#include "items/scroll_fireball.hpp"
#include "items/scroll_lightning.hpp"
#include "json.hpp"
#include "types/actor.hpp"
#include "types/fixture.hpp"
#include "types/item.hpp"
#include "types/map.hpp"
#include "types/messages.hpp"
#include "types/position.hpp"

inline void to_json(json& j, const tcod::ColorRGB& color) { j = {color.r, color.g, color.b}; }
inline void from_json(const json& j, tcod::ColorRGB& color) {
  j[0].get_to(color.r);
  j[1].get_to(color.g);
  j[2].get_to(color.b);
}

inline void to_json(json& j, const Position& pos) { j = {pos.x, pos.y}; }
inline void from_json(const json& j, Position& pos) {
  j[0].get_to(pos.x);
  j[1].get_to(pos.y);
}

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Fixture, name, ch, fg);

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(HealthPotion, count);
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ConfusionScroll, count, confuse_turns);
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(FireballScroll, count, range_squared, atk_damage);
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(LightningScroll, count, range_squared, atk_damage);

inline void to_json(json& j, const std::unique_ptr<Item>& item) {
  if (const auto* known = dynamic_cast<const HealthPotion*>(item.get()); known) {
    j["type"] = "HealthPotion";
    j["data"] = *known;
    return;
  }
  if (const auto* known = dynamic_cast<const ConfusionScroll*>(item.get()); known) {
    j["type"] = "ConfusionScroll";
    j["data"] = *known;
    return;
  }
  if (const auto* known = dynamic_cast<const FireballScroll*>(item.get()); known) {
    j["type"] = "FireballScroll";
    j["data"] = *known;
    return;
  }
  if (const auto* known = dynamic_cast<const LightningScroll*>(item.get()); known) {
    j["type"] = "LightningScroll";
    j["data"] = *known;
    return;
  }
  assert(0);
}
inline void from_json(const json& j, std::unique_ptr<Item>& item) {
  auto name = j.at("type").get<std::string>();
  if (name == "HealthPotion") {
    item = std::make_unique<HealthPotion>(j.at("data").get<HealthPotion>());
  } else if (name == "ConfusionScroll") {
    item = std::make_unique<ConfusionScroll>(j.at("data").get<ConfusionScroll>());
  } else if (name == "FireballScroll") {
    item = std::make_unique<FireballScroll>(j.at("data").get<FireballScroll>());
  } else if (name == "LightningScroll") {
    item = std::make_unique<LightningScroll>(j.at("data").get<LightningScroll>());
  } else {
    assert(0);
  }
}

namespace action {
inline void to_json(json& j, const std::unique_ptr<Action>& item) {
  if (!item) {
    j = nullptr;
    return;
  }
  if (const auto* known = dynamic_cast<const BasicAI*>(item.get()); known) {
    j["type"] = "BasicAI";
    return;
  }
  assert(0);
}
inline void from_json(const json& j, std::unique_ptr<Action>& item) {
  if (j.is_null()) return;
  auto name = j.at("type").get<std::string>();
  if (name == "BasicAI") {
    item = std::make_unique<BasicAI>();
  } else {
    assert(0);
  }
}
}  // namespace action

inline void to_json(json& j, const Stats& stats) {
  j["max_hp"] = stats.max_hp;
  j["hp"] = stats.hp;
  j["attack"] = stats.attack;
  j["defense"] = stats.defense;
  j["level"] = stats.level;
  j["xp"] = stats.xp;
  j["inventory"] = stats.inventory;
  j["confused_turns"] = stats.confused_turns;
}
inline void from_json(const json& j, Stats& stats) {
  j.at("max_hp").get_to(stats.max_hp);
  j.at("hp").get_to(stats.hp);
  j.at("attack").get_to(stats.attack);
  j.at("defense").get_to(stats.defense);
  if (j.contains("level")) j.at("level").get_to(stats.level);  // Migration.
  if (j.contains("xp")) j.at("xp").get_to(stats.xp);  // Migration.
  j.at("inventory").get_to(stats.inventory);
  j.at("confused_turns").get_to(stats.confused_turns);
}

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Actor, pos, name, ch, fg, stats, ai);

namespace util {
template <typename T>
inline void to_json(json& j, const util::Array2D<T>& array) {
  j = {{"shape", array.get_shape()}, {"data", array.get_container()}};
}
template <typename T>
inline void from_json(const json& j, util::Array2D<T>& array) {
  array = util::Array2D<T>{j.at("shape").get<std::array<int, 2>>()};
  j.at("data").get_to(array.get_container());
}
}  // namespace util

inline void to_json(json& j, const MapID& map_id) {
  j["name"] = map_id.name;
  j["level"] = map_id.level;
}
inline void from_json(const json& j, MapID& map_id) {
  j.at("name").get_to(map_id.name);
  j.at("level").get_to(map_id.level);
}

inline void to_json(json& j, const Map& map) {
  j["tiles"] = map.tiles;
  j["explored"] = map.explored;
  j["visible"] = map.visible;
  j["items"] = std::vector<std::pair<Position, const std::unique_ptr<Item>&>>(map.items.begin(), map.items.end());
  j["fixtures"] = map.fixtures;
  j["frozen_actors"] = map.frozen_actors;
}
inline void from_json(const json& j, Map& map) {
  j.at("tiles").get_to(map.tiles);
  j.at("explored").get_to(map.explored);
  j.at("visible").get_to(map.visible);
  auto items = j.at("items").get<std::vector<std::pair<Position, std::unique_ptr<Item>>>>();
  while (items.size()) {
    map.items.emplace(std::move(items.back()));
    items.pop_back();
  }
  if (j.contains("fixtures")) j.at("fixtures").get_to(map.fixtures);
  if (j.contains("frozen_actors")) j.at("frozen_actors").get_to(map.frozen_actors);
}

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Message, text, fg, count);
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(MessageLog, messages);

inline void to_json(json& j, const World& world) {
  std::stringstream rng{};
  rng << world.rng;
  j["actors"] = world.actors;
  j["maps"] = world.maps;
  j["rng"] = rng.str();
  j["schedule"] = world.schedule;
  j["log"] = world.log;
  j["current_map"] = world.current_map_id;
}

inline void from_json(const json& j, World& world) {
  j.at("actors").get_to(world.actors);
  for (auto& [actor_id, actor] : world.actors) actor.id = actor_id;
  if (!j.contains("current_map")) {  // Migrate.
    j.at("maps").at("main").get_to(world.maps[{"caves", 0}]);
    world.current_map_id = {"caves", 0};
  } else {
    j.at("maps").get_to(world.maps);
    j.at("current_map").get_to(world.current_map_id);
  }
  for (auto& [map_id, map] : world.maps) map.id = map_id;
  std::stringstream{j.at("rng").get<std::string>()} >> world.rng;
  j.at("schedule").get_to(world.schedule);
  j.at("log").get_to(world.log);
  if (!j.contains("active_actors")) {  // Migrate.
    for (auto id : world.schedule) world.active_actors.emplace(id);
  } else {
    j.at("active_actors").get_to(world.active_actors);
  }
}

inline auto save_world(const World& world, std::filesystem::path path) -> void {
  json data{};
  data["world"] = world;
  std::ofstream f{path};
  f << data << "\n";
  std::cout << "Game saved.\n";
#ifdef __EMSCRIPTEN__
  // clang-format off
  EM_ASM(
    FS.syncfs(false, function (err) {
      assert(!err);
      console.log("SyncFS finished.");
    });
  );
  // clang-format on
#endif
}

inline auto load_world(std::filesystem::path path) -> std::unique_ptr<World> {
  if (!std::filesystem::exists(path)) {
    std::cerr << "Save file does not exist:\n" << path << "\n";
    return nullptr;
  }
  try {
    std::fstream f{path};
    json data = json::parse(f);
    std::unique_ptr<World> world = std::make_unique<World>();
    data.at("world").get_to(*world);
    return world;
  } catch (const std::exception& exc) {
    std::cerr << "Failed to load world:\n" << exc.what() << "\n";
  }
  return nullptr;
}

inline auto save_world(const World& world) -> void {
  std::filesystem::create_directories("saves");
  return save_world(world, "saves/save.json");
}

inline auto load_world() -> std::unique_ptr<World> { return load_world("saves/save.json"); }
