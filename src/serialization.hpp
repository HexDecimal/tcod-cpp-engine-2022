#pragma once

#include <filesystem>
#include <fstream>
#include <libtcod/color.hpp>
#include <nlohmann/json.hpp>

#include "actions/ai_basic.hpp"
#include "items/health_potion.hpp"
#include "items/scroll_confusion.hpp"
#include "items/scroll_fireball.hpp"
#include "items/scroll_lightning.hpp"
#include "types/actor.hpp"
#include "types/item.hpp"
#include "types/map.hpp"
#include "types/messages.hpp"
#include "types/position.hpp"

using json = nlohmann::json;

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

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Stats, max_hp, hp, attack, defense, inventory, confused_turns);
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

inline void to_json(json& j, const Map& map) {
  j["tiles"] = map.tiles;
  j["explored"] = map.explored;
  j["visible"] = map.visible;
  j["items"] = std::vector<std::pair<Position, const std::unique_ptr<Item>&>>(map.items.begin(), map.items.end());
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
}

inline void from_json(const json& j, World& world) {
  j.at("actors").get_to(world.actors);
  for (auto& [actor_id, actor] : world.actors) actor.id = actor_id;
  j.at("maps").get_to(world.maps);
  std::stringstream{j.at("rng").get<std::string>()} >> world.rng;
  j.at("schedule").get_to(world.schedule);
  j.at("log").get_to(world.log);
}

inline auto save_world(const World& world, std::filesystem::path path) -> void {
  json data{};
  data["world"] = world;
  std::ofstream f{path};
  f << data << "\n";
}

inline auto load_world(std::filesystem::path path) -> std::unique_ptr<World> {
  std::fstream f{path};
  json data = json::parse(f);
  std::unique_ptr<World> world = std::make_unique<World>();
  data.at("world").get_to(*world);
  return world;
}
