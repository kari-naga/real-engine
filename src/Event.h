#pragma once

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include <tuple>

#include "LuaDB.h"

class Event {
  public:
    static auto Publish(const char *, luabridge::LuaRef) -> void;

    static auto Subscribe(const char *, luabridge::LuaRef, luabridge::LuaRef) -> void;

    static auto Unsubscribe(const char *, luabridge::LuaRef, luabridge::LuaRef) -> void;

    static auto ResolveEvents() -> void;

  private:
    static inline std::unordered_map<std::string, std::vector<std::pair<luabridge::LuaRef, luabridge::LuaRef>>> events;

    static inline std::vector<std::tuple<std::string, luabridge::LuaRef, luabridge::LuaRef>> subscribe_queue;

    static inline std::vector<std::tuple<std::string, luabridge::LuaRef, luabridge::LuaRef>> unsubscribe_queue;
};