#pragma once

#include <string>
#include <map>
#include <set>
#include <unordered_map>

#include "rapidjson/document.h"
#include "ldtk.hpp"

#include "Component.h"
#include "LuaDB.h"

class ComponentCmp {
  public:
    bool operator()(const Component *a, const Component *b) const {
        return a->key < b->key;
    }
};

class Actor {
  public:
    std::string actor_name;
    std::string template_name;
    std::map<std::string, Component> components;
    std::unordered_map<std::string, std::set<Component *, ComponentCmp>> type_to_components;
    std::map<std::string, Component *> collision_enter_components;
    std::map<std::string, Component *> collision_exit_components;
    std::map<std::string, Component *> trigger_enter_components;
    std::map<std::string, Component *> trigger_exit_components;
    size_t id = 0;
    bool persistent = false;

    auto ParseActor(const rapidjson::Value &) -> void;

    auto ParseLdtkEntity(const int, const std::unordered_map<int64_t, std::string> &, const double, const ldtk::EntityInstance &) -> void;

    auto ParseLdtkTile(const int, const std::string &, const double, const int64_t, const bool, const ldtk::TileInstance &, const std::string &) -> void;

    auto GetName() -> std::string;

    auto GetID() -> int;

    auto GetComponentByKey(const char *) -> luabridge::LuaRef;

    auto GetComponent(const char *) -> luabridge::LuaRef;

    auto GetComponents(const char *) -> luabridge::LuaRef;

    auto InjectConvenienceReferences(Component &) -> void;

    auto Copy() const -> Actor;

    auto BuildDataStructures() -> void;

    auto AddComponent(const char *) -> luabridge::LuaRef;

    auto RemoveComponent(luabridge::LuaRef) -> void;

    static auto FinishAddingComponents() -> void;

    static auto FinishRemovingComponents() -> void;

  private:
    static inline auto runtime_component_counter = size_t{0};
    static inline auto add_component_queue = std::vector<Component *>();
    static inline auto remove_component_queue = std::vector<Component *>();

    auto ParseComponents(const rapidjson::Value &) -> void;

    auto FindComponentByRef(const luabridge::LuaRef &) -> Component *;
};
