#include "Event.h"

#include <algorithm>

auto Event::Publish(const char *event_type, luabridge::LuaRef event_object) -> void {
    if (const auto it = events.find(event_type); it != events.end()) {
        for (const auto &[component, func] : it->second) {
            func(component, event_object);
        }
    }
}

auto Event::Subscribe(const char *event_type, luabridge::LuaRef component, luabridge::LuaRef function) -> void {
    subscribe_queue.push_back({event_type, component, function});
}

auto Event::Unsubscribe(const char *event_type, luabridge::LuaRef component, luabridge::LuaRef function) -> void {
    unsubscribe_queue.push_back({event_type, component, function});
}

auto Event::ResolveEvents() -> void {
    for (const auto &[event_type, component, function] : unsubscribe_queue) {
        if (const auto event_it = events.find(event_type); event_it != events.end()) {
            if (const auto sub_it = std::find(event_it->second.begin(), event_it->second.end(), std::pair<luabridge::LuaRef, luabridge::LuaRef>(component, function)); sub_it != event_it->second.end()) {
                event_it->second.erase(sub_it);
            }
        }
    }
    unsubscribe_queue.clear();
    for (const auto &[event_type, component, function] : subscribe_queue) {
        events[event_type].push_back({component, function});
    }
    subscribe_queue.clear();
}
