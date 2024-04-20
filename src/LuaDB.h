#pragma once

#include <string>

#include "lua/lua.hpp"
#include "LuaBridge/LuaBridge.h"

class LuaDB {
  public:
    static auto Init() -> void;

    static auto GetLuaState() -> lua_State *;
    
    static auto EstablishInheritance(luabridge::LuaRef &, const luabridge::LuaRef &) -> void;

    static auto ReportError(const std::string &, const luabridge::LuaException &) -> void;

  private:
    static inline lua_State *lua_state;

    static auto Log(const char *) -> void;
    
    static auto LogError(const char *) -> void;
};
