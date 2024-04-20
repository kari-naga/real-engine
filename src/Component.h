#pragma once

#include <string>
#include <memory>

#include "rapidjson/document.h"

#include "LuaDB.h"

class Component {
  public:
    std::shared_ptr<luabridge::LuaRef> ref;
    std::string type;
    std::string key;
    size_t actor_id;

    bool hasStart = false;
    bool hasUpdate = false;
    bool hasLateUpdate = false;
    bool hasDestroy = false;
    bool hasOnCollisionEnter = false;
    bool hasOnCollisionExit = false;
    bool hasOnTriggerEnter = true;
    bool hasOnTriggerExit = false;

    inline auto IsEnabled() -> bool {
        return (*ref)["enabled"];
    }

    inline auto SetEnabled(bool enabled) -> void {
        (*ref)["enabled"] = enabled;
    }

    inline auto ApplyOverride(const std::string &property, const rapidjson::Value &val) {
        if (val.IsString()) {
            (*ref)[property] = val.GetString();
        } else if (val.IsInt()) {
            (*ref)[property] = val.GetInt();
        } else if (val.IsFloat()) {
            (*ref)[property] = val.GetFloat();
        } else if (val.IsBool()) {
            (*ref)[property] = val.GetBool();
        }
    }
};
