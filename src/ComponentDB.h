#pragma once

#include <filesystem>
#include <unordered_set>

#include "Component.h"
#include "LuaDB.h"
#include "Rigidbody.h"

class ComponentDB {
  public:
    static inline auto LoadComponent(const std::string &key, const std::string &component_name) {
        if (native_components.find(component_name) != native_components.end()) {
            return MakeNativeComponentInstance(key, component_name);
        }
        if (loaded_components.find(component_name) == loaded_components.end()) {
            auto component_file = "core/component_types/" + component_name + ".lua";
            if (!std::filesystem::exists(component_file)) {
                component_file = "resources/component_types/" + component_name + ".lua";
                if (!std::filesystem::exists(component_file)) {
                    std::cout << "error: failed to locate component " << component_name;
                    exit(0);
                }
            }
            if (luaL_dofile(LuaDB::GetLuaState(), component_file.c_str()) != LUA_OK) {
                std::cout << "problem with lua file " << component_name;
                exit(0);
            };
            loaded_components.insert(component_name);
        }
        return MakeComponentInstance(key, component_name);
    }

    static inline auto CloneComponent(const Component& component, const std::string &key) {
        if (native_components.find(component.type) != native_components.end()) {
            return CloneNativeComponent(component, key);
        }
        auto new_component = Component{};
        new_component.ref = std::make_shared<luabridge::LuaRef>(luabridge::newTable(LuaDB::GetLuaState()));
        LuaDB::EstablishInheritance(*new_component.ref, *component.ref);
        return MakeComponent(new_component, key, component.type);
    }

  private:
    static inline std::unordered_set<std::string> loaded_components;
    static inline std::unordered_set<std::string> native_components = {"Rigidbody"};

    static inline auto MakeComponent(Component &component, const std::string &key, const std::string &component_name) -> Component {
        component.type = component_name;
        component.key = key;
        (*component.ref)["key"] = key;
        (*component.ref)["enabled"] = true;
        component.hasStart = !(*component.ref)["OnStart"].isNil();
        component.hasUpdate = !(*component.ref)["OnUpdate"].isNil();
        component.hasLateUpdate = !(*component.ref)["OnLateUpdate"].isNil();
        component.hasDestroy = !(*component.ref)["OnDestroy"].isNil();
        component.hasOnCollisionEnter = !(*component.ref)["OnCollisionEnter"].isNil();
        component.hasOnCollisionExit = !(*component.ref)["OnCollisionExit"].isNil();
        component.hasOnTriggerEnter = !(*component.ref)["OnTriggerEnter"].isNil();
        component.hasOnTriggerExit = !(*component.ref)["OnTriggerExit"].isNil();
        return component;
    }

    static inline auto MakeComponentInstance(const std::string &key, const std::string &component_name) -> Component {
        const auto lua_state = LuaDB::GetLuaState();
        const auto parent_table = luabridge::getGlobal(lua_state, component_name.c_str());
        auto component = Component{};
        component.ref = std::make_shared<luabridge::LuaRef>(luabridge::newTable(lua_state));
        LuaDB::EstablishInheritance(*component.ref, parent_table);
        return MakeComponent(component, key, component_name);
    }

    static inline auto MakeNativeComponentInstance(const std::string &key, const std::string &component_name) -> Component {
        const auto lua_state = LuaDB::GetLuaState();
        auto component = Component{};
        if (component_name == "Rigidbody") {
            component.ref = std::make_shared<luabridge::LuaRef>(luabridge::LuaRef(lua_state, Rigidbody::MakeRigidbody()));
        }
        return MakeComponent(component, key, component_name);
    }

    static inline auto CloneNativeComponent(const Component& original_component, const std::string &key) -> Component {
        const auto lua_state = LuaDB::GetLuaState();
        auto component = Component{};
        if (original_component.type == "Rigidbody") {
            auto rb = Rigidbody::MakeRigidbody();
            const Rigidbody *original_rb = *original_component.ref;
            rb->x = original_rb->x;
            rb->y = original_rb->y;
            rb->body_type = original_rb->body_type;
            rb->precise = original_rb->precise;
            rb->gravity_scale = original_rb->gravity_scale;
            rb->density = original_rb->density;
            rb->angular_friction = original_rb->angular_friction;
            rb->rotation = original_rb->rotation;
            rb->has_collider = original_rb->has_collider;
            rb->has_trigger = original_rb->has_trigger;
            rb->collider_type = original_rb->collider_type;
            rb->width = original_rb->width;
            rb->height = original_rb->height;
            rb->radius = original_rb->radius;
            rb->friction = original_rb->friction;
            rb->bounciness = original_rb->bounciness;
            rb->trigger_type = original_rb->trigger_type;
            rb->trigger_width = original_rb->trigger_width;
            rb->trigger_height = original_rb->trigger_height;
            rb->trigger_radius = original_rb->trigger_radius;
            component.ref = std::make_shared<luabridge::LuaRef>(luabridge::LuaRef(lua_state, rb));
        }
        return MakeComponent(component, key, original_component.type);
    }
};
