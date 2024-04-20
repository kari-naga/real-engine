#include "Actor.h"

#include "ComponentDB.h"
#include "Engine.h"

auto Actor::ParseActor(const rapidjson::Value &val) -> void {
    for (const auto &[actor_key, actor_val] : val.GetObject()) {
        if (actor_key == "name") {
            actor_name = actor_val.GetString();
        } else if (actor_key == "template") {
            template_name = actor_val.GetString();
        } else if (actor_key == "components") {
            ParseComponents(actor_val);
        }
    }
}

auto Actor::ParseLdtkEntity(const int layer, const std::unordered_map<int64_t, std::string> &tilesets, const double opacity, const ldtk::EntityInstance &entity) -> void {
    actor_name = entity.get_identifier();
    const auto &px = entity.get_px();
    const auto &pivot = entity.get_pivot();
    const auto tile_opt = entity.get_tile();
    if (tile_opt) {
        auto tile_renderer_doc = rapidjson::Document();
        tile_renderer_doc.Parse("{\"tile_renderer\": {\"type\": \"TileRenderer\"}}");
        auto &tile_renderer_allocator = tile_renderer_doc.GetAllocator();
        auto &tile_renderer = tile_renderer_doc["tile_renderer"];
        const auto &tile = tile_opt.value();
        tile_renderer.AddMember("tileset", tilesets.at(tile.get_tileset_uid()), tile_renderer_allocator);
        tile_renderer.AddMember("x", (px[0] + tile.get_w() / 2) / static_cast<float>(Engine::config.pixels_per_meter), tile_renderer_allocator);
        tile_renderer.AddMember("y", (px[1] + tile.get_h() / 2) / static_cast<float>(Engine::config.pixels_per_meter), tile_renderer_allocator);
        tile_renderer.AddMember("w", entity.get_width(), tile_renderer_allocator);
        tile_renderer.AddMember("h", entity.get_height(), tile_renderer_allocator);
        tile_renderer.AddMember("tx", tile.get_x(), tile_renderer_allocator);
        tile_renderer.AddMember("ty", tile.get_y(), tile_renderer_allocator);
        tile_renderer.AddMember("tw", tile.get_w(), tile_renderer_allocator);
        tile_renderer.AddMember("th", tile.get_h(), tile_renderer_allocator);
        tile_renderer.AddMember("a", static_cast<int>(opacity * 255), tile_renderer_allocator);
        tile_renderer.AddMember("sorting_order", -layer, tile_renderer_allocator);
        ParseComponents(tile_renderer_doc);
    }
    auto added_rigidbody = false;
    auto rigidbody_doc = rapidjson::Document();
    rigidbody_doc.Parse("{\"rb\": {\"type\": \"Rigidbody\"}}");
    auto &rigidbody_allocator = rigidbody_doc.GetAllocator();
    auto &rigidbody = rigidbody_doc["rb"];
    rigidbody.AddMember("x", (px[0] + entity.get_width() / 2) / static_cast<float>(Engine::config.pixels_per_meter), rigidbody_allocator);
    rigidbody.AddMember("y", (px[1] + entity.get_height() / 2) / static_cast<float>(Engine::config.pixels_per_meter), rigidbody_allocator);
    auto components_str = std::string();
    for (const auto &field : entity.get_field_instances()) {
        const auto &identifier = field.get_identifier();
        const auto &type = field.get_type();
        if (identifier == "components" && (type == "String" || type == "Multilines")) {
            components_str = field.get_value().template get<std::string>();
        } else if (identifier == "body_type" && type == "String") {
            added_rigidbody = true;
            rigidbody.AddMember("body_type", field.get_value().template get<std::string>(), rigidbody_allocator);
        } else if (identifier == "precise" && type == "Bool") {
            added_rigidbody = true;
            rigidbody.AddMember("precise", field.get_value().template get<bool>(), rigidbody_allocator);
        } else if (identifier == "gravity_scale" && type == "Float") {
            added_rigidbody = true;
            rigidbody.AddMember("gravity_scale", field.get_value().template get<float>(), rigidbody_allocator);
        } else if (identifier == "density" && type == "Float") {
            added_rigidbody = true;
            rigidbody.AddMember("density", field.get_value().template get<float>(), rigidbody_allocator);
        } else if (identifier == "angular_friction" && type == "Float") {
            added_rigidbody = true;
            rigidbody.AddMember("angular_friction", field.get_value().template get<float>(), rigidbody_allocator);
        } else if (identifier == "rotation" && type == "Float") {
            added_rigidbody = true;
            rigidbody.AddMember("rotation", field.get_value().template get<float>(), rigidbody_allocator);
        } else if (identifier == "has_collider" && type == "Bool") {
            added_rigidbody = true;
            rigidbody.AddMember("has_collider", field.get_value().template get<bool>(), rigidbody_allocator);
        } else if (identifier == "has_trigger" && type == "Bool") {
            added_rigidbody = true;
            rigidbody.AddMember("has_trigger", field.get_value().template get<bool>(), rigidbody_allocator);
        } else if (identifier == "collider_type" && type == "String") {
            added_rigidbody = true;
            rigidbody.AddMember("collider_type", field.get_value().template get<std::string>(), rigidbody_allocator);
        } else if (identifier == "width" && type == "Float") {
            added_rigidbody = true;
            rigidbody.AddMember("width", field.get_value().template get<float>(), rigidbody_allocator);
        } else if (identifier == "height" && type == "Float") {
            added_rigidbody = true;
            rigidbody.AddMember("height", field.get_value().template get<float>(), rigidbody_allocator);
        } else if (identifier == "radius" && type == "Float") {
            added_rigidbody = true;
            rigidbody.AddMember("radius", field.get_value().template get<float>(), rigidbody_allocator);
        } else if (identifier == "friction" && type == "Float") {
            added_rigidbody = true;
            rigidbody.AddMember("friction", field.get_value().template get<float>(), rigidbody_allocator);
        } else if (identifier == "bounciness" && type == "Float") {
            added_rigidbody = true;
            rigidbody.AddMember("bounciness", field.get_value().template get<float>(), rigidbody_allocator);
        } else if (identifier == "trigger_type" && type == "String") {
            added_rigidbody = true;
            rigidbody.AddMember("trigger_type", field.get_value().template get<std::string>(), rigidbody_allocator);
        } else if (identifier == "trigger_width" && type == "Float") {
            added_rigidbody = true;
            rigidbody.AddMember("trigger_width", field.get_value().template get<float>(), rigidbody_allocator);
        } else if (identifier == "trigger_height" && type == "Float") {
            added_rigidbody = true;
            rigidbody.AddMember("trigger_height", field.get_value().template get<float>(), rigidbody_allocator);
        } else if (identifier == "trigger_radius" && type == "Float") {
            added_rigidbody = true;
            rigidbody.AddMember("trigger_radius", field.get_value().template get<float>(), rigidbody_allocator);
        } else if (identifier == "has_auto_rigidbody" && type == "Bool" && field.get_value().template get<bool>()) {
            added_rigidbody = true;
            rigidbody.AddMember("width", entity.get_width() / static_cast<float>(Engine::config.pixels_per_meter), rigidbody_allocator);
            rigidbody.AddMember("height", entity.get_height() / static_cast<float>(Engine::config.pixels_per_meter), rigidbody_allocator);
        }
    }
    if (added_rigidbody) {
        ParseComponents(rigidbody_doc);
    }
    if (components_str != "") {
        auto components_doc = rapidjson::Document();
        if (components_doc.Parse(components_str.c_str()).HasParseError()) {
            std::cout << "error: failed to parse components for entity " << entity.get_identifier();
            exit(0);
        };
        ParseComponents(components_doc);
    }
}

auto Actor::ParseLdtkTile(const int layer, const std::string &tileset, const double opacity, const int64_t grid_size, const bool bg, const ldtk::TileInstance &tile, const std::string &name) -> void {
    actor_name = name;
    const auto &src = tile.get_src();
    const auto &px = tile.get_px();
    const auto &f = tile.get_f();
    auto tile_renderer_doc = rapidjson::Document();
    tile_renderer_doc.Parse("{\"tile_renderer\": {\"type\": \"TileRenderer\"}}");
    auto &tile_renderer_allocator = tile_renderer_doc.GetAllocator();
    auto &tile_renderer = tile_renderer_doc["tile_renderer"];
    tile_renderer.AddMember("tileset", tileset, tile_renderer_allocator);
    tile_renderer.AddMember("x", (px[0] + grid_size / 2) / static_cast<float>(Engine::config.pixels_per_meter), tile_renderer_allocator);
    tile_renderer.AddMember("y", (px[1] + grid_size / 2) / static_cast<float>(Engine::config.pixels_per_meter), tile_renderer_allocator);
    tile_renderer.AddMember("w", grid_size, tile_renderer_allocator);
    tile_renderer.AddMember("h", grid_size, tile_renderer_allocator);
    tile_renderer.AddMember("tx", src[0], tile_renderer_allocator);
    tile_renderer.AddMember("ty", src[1], tile_renderer_allocator);
    tile_renderer.AddMember("tw", grid_size, tile_renderer_allocator);
    tile_renderer.AddMember("th", grid_size, tile_renderer_allocator);
    tile_renderer.AddMember("tfx", f & 1 ? true : false, tile_renderer_allocator);
    tile_renderer.AddMember("tfy", f & 2 ? true : false, tile_renderer_allocator);
    tile_renderer.AddMember("a", static_cast<int>(opacity * 255), tile_renderer_allocator);
    tile_renderer.AddMember("sorting_order", -layer, tile_renderer_allocator);
    ParseComponents(tile_renderer_doc);
    if (!bg) {
        auto rigidbody_doc = rapidjson::Document();
        rigidbody_doc.Parse("{\"rb\": {\"type\": \"Rigidbody\", \"body_type\": \"static\"}}");
        auto &rigidbody_allocator = rigidbody_doc.GetAllocator();
        auto &rigidbody = rigidbody_doc["rb"];
        rigidbody.AddMember("x", (px[0] + grid_size / 2) / static_cast<float>(Engine::config.pixels_per_meter), rigidbody_allocator);
        rigidbody.AddMember("y", (px[1] + grid_size / 2) / static_cast<float>(Engine::config.pixels_per_meter), rigidbody_allocator);
        rigidbody.AddMember("friction", Engine::config.default_tile_friction, rigidbody_allocator);
        rigidbody.AddMember("bounciness", Engine::config.default_tile_bounciness, rigidbody_allocator);
        rigidbody.AddMember("width", grid_size / static_cast<float>(Engine::config.pixels_per_meter), rigidbody_allocator);
        rigidbody.AddMember("height", grid_size / static_cast<float>(Engine::config.pixels_per_meter), rigidbody_allocator);
        ParseComponents(rigidbody_doc);
    }
}

auto Actor::GetName() -> std::string {
    return actor_name;
}

auto Actor::GetID() -> int {
    return static_cast<int>(id);
}

auto Actor::GetComponentByKey(const char *key) -> luabridge::LuaRef {
    if (const auto it = components.find(key); it != components.end() && it->second.IsEnabled()) {
        return *it->second.ref;
    }
    return luabridge::LuaRef(LuaDB::GetLuaState());
}

auto Actor::GetComponent(const char *type) -> luabridge::LuaRef {
    if (const auto it = type_to_components.find(type); it != type_to_components.end() && !it->second.empty() && (*it->second.begin())->IsEnabled()) {
        return *(*it->second.begin())->ref;
    }
    return luabridge::LuaRef(LuaDB::GetLuaState());
}

auto Actor::GetComponents(const char *type) -> luabridge::LuaRef {
    const auto found_components = luabridge::newTable(LuaDB::GetLuaState());
    if (const auto it = type_to_components.find(type); it != type_to_components.end()) {
        auto i = 1;
        for (const auto &component : it->second) {
            if (component->IsEnabled()) {
                found_components[i] = *component->ref;
                ++i;
            }
        }
    }
    return found_components;
}

auto Actor::InjectConvenienceReferences(Component &component) -> void {
    (*component.ref)["actor"] = this;
}

auto Actor::Copy() const -> Actor {
    auto copy = Actor{};
    copy.actor_name = actor_name;
    copy.template_name = template_name;
    for (const auto &[key, component] : components) {
        copy.components.insert({key, ComponentDB::CloneComponent(component, key)});
    }
    return copy;
}

auto Actor::BuildDataStructures() -> void {
    type_to_components.clear();
    for (auto &[key, component] : components) {
        type_to_components[component.type].insert(&component);
        if (component.hasOnCollisionEnter) {
            collision_enter_components.insert({key, &component});
        }
        if (component.hasOnCollisionExit) {
            collision_exit_components.insert({key, &component});
        }
        if (component.hasOnTriggerEnter) {
            trigger_enter_components.insert({key, &component});
        }
        if (component.hasOnTriggerExit) {
            trigger_exit_components.insert({key, &component});
        }
        component.actor_id = id;
        InjectConvenienceReferences(component);
    }
}

auto Actor::AddComponent(const char *type) -> luabridge::LuaRef { // move to queue
    if (type == nullptr) {
        return luabridge::LuaRef(LuaDB::GetLuaState());
    }
    const auto key = "r" + std::to_string(runtime_component_counter++);
    auto component = ComponentDB::LoadComponent(key, type);
    component.actor_id = id;
    InjectConvenienceReferences(component);
    const auto it = components.insert({key, component});
    type_to_components[type].insert(&it.first->second);
    if (component.hasOnCollisionEnter) {
        collision_enter_components.insert({key, &it.first->second});
    }
    if (component.hasOnCollisionExit) {
        collision_exit_components.insert({key, &it.first->second});
    }
    if (component.hasOnTriggerEnter) {
        trigger_enter_components.insert({key, &it.first->second});
    }
    if (component.hasOnTriggerExit) {
        trigger_exit_components.insert({key, &it.first->second});
    }
    add_component_queue.push_back(&it.first->second);
    return *it.first->second.ref;
}

auto Actor::RemoveComponent(luabridge::LuaRef ref) -> void {
    if (auto component = FindComponentByRef(ref); component != nullptr) {
        const auto it = std::find(add_component_queue.begin(), add_component_queue.end(), component);
        if (it != add_component_queue.end()) {
            add_component_queue.erase(it);
        }
        component->SetEnabled(false);
        if (component->hasDestroy) {
            Engine::scene.destroy_queue.insert(component);
        }
        type_to_components[component->type].erase(component);
        if (component->hasOnCollisionEnter) {
            collision_enter_components.erase(component->key);
        }
        if (component->hasOnCollisionExit) {
            collision_exit_components.erase(component->key);
        }
        if (component->hasOnTriggerEnter) {
            trigger_enter_components.erase(component->key);
        }
        if (component->hasOnTriggerExit) {
            trigger_exit_components.erase(component->key);
        }
        remove_component_queue.push_back(component);
    }
}

auto Actor::FinishAddingComponents() -> void {
    for (const auto component : add_component_queue) {
        Engine::scene.RegisterComponent(*component);
    }
    add_component_queue.clear();
}

auto Actor::FinishRemovingComponents() -> void {
    for (auto component : remove_component_queue) {
        Engine::scene.UnregisterComponent(component);
    }
    remove_component_queue.clear();
}

auto Actor::ParseComponents(const rapidjson::Value &val) -> void {
    for (const auto &[component_key, component_obj] : val.GetObject()) {
        const auto key = component_key.GetString();
        Component component;
        if (const auto it = components.find(key); it != components.end()) {
            component = ComponentDB::CloneComponent(it->second, key);
        } else {
            component = ComponentDB::LoadComponent(key, component_obj["type"].GetString());
        }
        for (const auto &[property_name, property_value] : component_obj.GetObject()) {
            if (property_name != "type") {
                component.ApplyOverride(property_name.GetString(), property_value);
            }
        }
        components[key] = component;
    }
}

auto Actor::FindComponentByRef(const luabridge::LuaRef &ref) -> Component * {
    for (auto &[key, component] : components) {
        if (*component.ref == ref) {
            return &component;
        }
    }
    return nullptr;
}
