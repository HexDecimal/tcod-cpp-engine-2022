#pragma once

#include <libtcod.hpp>
#include <string>

#include "../constants.hpp"

struct Message {
  std::string text;
  tcod::ColorRGB fg;
  int count = 1;
};

struct MessageLog {
  void append(std::string text, tcod::ColorRGB fg = constants::TEXT_COLOR_DEFAULT) {
    if (!messages.empty() && messages.back().text == text && messages.back().fg == fg) {
      ++messages.back().count;
      return;
    }
    messages.emplace_back(Message{text, fg, 1});
  };
  std::vector<Message> messages;
};
