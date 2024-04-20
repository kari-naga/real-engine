#include "LuaDB.h"

#include <algorithm>
#include <iostream>

#include "box2d/box2d.h"

#include "Actor.h"
#include "Engine.h"
#include "Input.h"
#include "TextDB.h"
#include "Time.h"
#include "AudioDB.h"
#include "TextureDB.h"
#include "Rigidbody.h"
#include "Physics.h"
#include "Event.h"

auto LuaDB::Init() -> void {
    lua_state = luaL_newstate();
    luaL_openlibs(lua_state);

    // Vec2
    luabridge::getGlobalNamespace(lua_state)
        .beginClass<glm::vec2>("vec2")
        .addProperty("x", &glm::vec2::x)
        .addProperty("y", &glm::vec2::y)
        .endClass();

    // Vector2
    luabridge::getGlobalNamespace(lua_state)
        .beginClass<b2Vec2>("Vector2")
        .addConstructor<void (*)(float, float)>()
        .addProperty("x", &b2Vec2::x)
        .addProperty("y", &b2Vec2::y)
        .addFunction("Normalize", &b2Vec2::Normalize)
        .addFunction("Length", &b2Vec2::Length)
        .addFunction("__add", &b2Vec2::operator_add)
        .addFunction("__sub", &b2Vec2::operator_sub)
        .addFunction("__mul", &b2Vec2::operator_mul)
        .addStaticFunction("Distance", static_cast<float (*)(const b2Vec2 &, const b2Vec2 &)>(&b2Distance))
        .addStaticFunction("Dot", static_cast<float (*)(const b2Vec2 &, const b2Vec2 &)>(&b2Dot))
        .endClass();

    // Rigidbody
    luabridge::getGlobalNamespace(lua_state)
        .beginClass<Rigidbody>("Rigidbody")
        .addProperty("type", &Rigidbody::type)
        .addProperty("key", &Rigidbody::key)
        .addProperty("actor", &Rigidbody::actor)
        .addProperty("enabled", &Rigidbody::enabled)
        .addProperty("x", &Rigidbody::x)
        .addProperty("y", &Rigidbody::y)
        .addProperty("body_type", &Rigidbody::body_type)
        .addProperty("precise", &Rigidbody::precise)
        .addProperty("gravity_scale", &Rigidbody::gravity_scale)
        .addProperty("density", &Rigidbody::density)
        .addProperty("angular_friction", &Rigidbody::angular_friction)
        .addProperty("rotation", &Rigidbody::rotation)
        .addProperty("has_collider", &Rigidbody::has_collider)
        .addProperty("has_trigger", &Rigidbody::has_trigger)
        .addProperty("collider_type", &Rigidbody::collider_type)
        .addProperty("width", &Rigidbody::width)
        .addProperty("height", &Rigidbody::height)
        .addProperty("radius", &Rigidbody::radius)
        .addProperty("friction", &Rigidbody::friction)
        .addProperty("bounciness", &Rigidbody::bounciness)
        .addProperty("trigger_type", &Rigidbody::trigger_type)
        .addProperty("trigger_width", &Rigidbody::trigger_width)
        .addProperty("trigger_height", &Rigidbody::trigger_height)
        .addProperty("trigger_radius", &Rigidbody::trigger_radius)
        .addFunction("GetPosition", &Rigidbody::GetPosition)
        .addFunction("GetRotation", &Rigidbody::GetRotation)
        .addFunction("GetVelocity", &Rigidbody::GetVelocity)
        .addFunction("GetAngularVelocity", &Rigidbody::GetAngularVelocity)
        .addFunction("GetGravityScale", &Rigidbody::GetGravityScale)
        .addFunction("GetUpDirection", &Rigidbody::GetUpDirection)
        .addFunction("GetRightDirection", &Rigidbody::GetRightDirection)
        .addFunction("AddForce", &Rigidbody::AddForce)
        .addFunction("AddTorque", &Rigidbody::AddTorque)
        .addFunction("AddLinearImpulse", &Rigidbody::AddLinearImpulse)
        .addFunction("AddAngularImpulse", &Rigidbody::AddAngularImpulse)
        .addFunction("SetVelocity", &Rigidbody::SetVelocity)
        .addFunction("SetPosition", &Rigidbody::SetPosition)
        .addFunction("SetRotation", &Rigidbody::SetRotation)
        .addFunction("SetAngularVelocity", &Rigidbody::SetAngularVelocity)
        .addFunction("SetGravityScale", &Rigidbody::SetGravityScale)
        .addFunction("SetUpDirection", &Rigidbody::SetUpDirection)
        .addFunction("SetRightDirection", &Rigidbody::SetRightDirection)
        .addFunction("OnStart", &Rigidbody::OnStart)
        .addFunction("OnDestroy", &Rigidbody::OnDestroy)
        .endClass();
    luabridge::getGlobalNamespace(lua_state)
        .beginClass<RigidbodyCollision>("Collision")
        .addProperty("other", &RigidbodyCollision::other)
        .addProperty("point", &RigidbodyCollision::point)
        .addProperty("relative_velocity", &RigidbodyCollision::relative_velocity)
        .addProperty("normal", &RigidbodyCollision::normal)
        .endClass();
    luabridge::getGlobalNamespace(lua_state)
        .beginClass<RaycastHitResult>("HitResult")
        .addProperty("actor", &RaycastHitResult::actor)
        .addProperty("point", &RaycastHitResult::point)
        .addProperty("normal", &RaycastHitResult::normal)
        .addProperty("is_trigger", &RaycastHitResult::is_trigger)
        .endClass();

    // Debug
    luabridge::getGlobalNamespace(lua_state)
        .beginNamespace("Debug")
        .addFunction("Log", &Log)
        .addFunction("LogError", &LogError)
        .endNamespace();

    // Actor
    luabridge::getGlobalNamespace(lua_state)
        .beginClass<Actor>("Actor")
        .addFunction("GetName", &Actor::GetName)
        .addFunction("GetID", &Actor::GetID)
        .addFunction("GetComponentByKey", &Actor::GetComponentByKey)
        .addFunction("GetComponent", &Actor::GetComponent)
        .addFunction("GetComponents", &Actor::GetComponents)
        .addFunction("AddComponent", &Actor::AddComponent)
        .addFunction("RemoveComponent", &Actor::RemoveComponent)
        .endClass();
    luabridge::getGlobalNamespace(lua_state)
        .beginNamespace("Actor")
        .addFunction("Find", &Engine::FindActor)
        .addFunction("FindAll", &Engine::FindAllActors)
        .addFunction("Instantiate", &Engine::InstantiateActor)
        .addFunction("Destroy", &Engine::DestroyActor)
        .endNamespace();

    // Application
    luabridge::getGlobalNamespace(lua_state)
        .beginNamespace("Application")
        .addFunction("GetFrame", &Engine::GetFrame)
        .addFunction("Quit", &Engine::Quit)
        .addFunction("Sleep", &Engine::Sleep)
        .addFunction("OpenURL", &Engine::OpenURL)
        .endNamespace();

    // Input
    luabridge::getGlobalNamespace(lua_state)
        .beginNamespace("Input")
        .addFunction("GetKey", &Input::GetKey)
        .addFunction("GetKeyDown", &Input::GetKeyDown)
        .addFunction("GetKeyUp", &Input::GetKeyUp)
        .addFunction("GetMousePosition", &Input::GetMousePosition)
        .addFunction("GetMouseButton", &Input::GetMouseButton)
        .addFunction("GetMouseButtonDown", &Input::GetMouseButtonDown)
        .addFunction("GetMouseButtonUp", &Input::GetMouseButtonUp)
        .addFunction("GetMouseScrollDelta", &Input::GetMouseScrollDelta)
        .endNamespace();

    // Text
    luabridge::getGlobalNamespace(lua_state)
        .beginNamespace("Text")
        .addFunction("Draw", &TextDB::DrawText)
        .endNamespace();

    // Audio
    luabridge::getGlobalNamespace(lua_state)
        .beginNamespace("Audio")
        .addFunction("Play", &AudioDB::PlayAudio)
        .addFunction("Halt", &AudioDB::HaltAudio)
        .addFunction("SetVolume", &AudioDB::SetVolume)
        .endNamespace();

    // Image
    luabridge::getGlobalNamespace(lua_state)
        .beginNamespace("Image")
        .addFunction("DrawUI", &TextureDB::DrawUI)
        .addFunction("DrawUIEx", &TextureDB::DrawUIEx)
        .addFunction("Draw", &TextureDB::DrawImage)
        .addFunction("DrawEx", &TextureDB::DrawImageEx)
        .addFunction("DrawTile", &TextureDB::DrawTile)
        .addFunction("DrawTileEx", &TextureDB::DrawTileEx)
        .addFunction("DrawPixel", &TextureDB::DrawPixel)
        .endNamespace();

    // Image
    luabridge::getGlobalNamespace(lua_state)
        .beginNamespace("Camera")
        .addFunction("SetPosition", &Engine::SetCameraPosition)
        .addFunction("GetPositionX", &Engine::GetCameraPositionX)
        .addFunction("GetPositionY", &Engine::GetCameraPositionY)
        .addFunction("SetZoom", &Engine::SetCameraZoom)
        .addFunction("GetZoom", &Engine::GetCameraZoom)
        .endNamespace();

    // Scene
    luabridge::getGlobalNamespace(lua_state)
        .beginNamespace("Scene")
        .addFunction("Load", &Engine::LoadScene)
        .addFunction("GetCurrent", &Engine::GetCurrentScene)
        .addFunction("DontDestroy", &Engine::DontDestroy)
        .endNamespace();

    // Physics
    luabridge::getGlobalNamespace(lua_state)
        .beginNamespace("Physics")
        .addFunction("Raycast", &Physics::Raycast)
        .addFunction("RaycastAll", &Physics::RaycastAll)
        .endNamespace();

    // Event
    luabridge::getGlobalNamespace(lua_state)
        .beginNamespace("Event")
        .addFunction("Publish", &Event::Publish)
        .addFunction("Subscribe", &Event::Subscribe)
        .addFunction("Unsubscribe", &Event::Unsubscribe)
        .endNamespace();

    // Time
    luabridge::getGlobalNamespace(lua_state)
        .beginNamespace("Time")
        .addFunction("DeltaTime", &Time::DeltaTime)
        .addFunction("UnscaledDeltaTime", &Time::UnscaledDeltaTime)
        .addFunction("GetTime", &Time::GetTime)
        .addFunction("GetUnscaledTime", &Time::GetUnscaledTime)
        .addFunction("GetTimeScale", &Time::GetTimeScale)
        .addFunction("SetTimeScale", &Time::SetTimeScale)
        .endNamespace();
}

auto LuaDB::GetLuaState() -> lua_State * {
    return lua_state;
}

auto LuaDB::EstablishInheritance(luabridge::LuaRef &instance_table, const luabridge::LuaRef &parent_table) -> void {
    luabridge::LuaRef new_metatable = luabridge::newTable(lua_state);
    new_metatable["__index"] = parent_table;
    instance_table.push(lua_state);
    new_metatable.push(lua_state);
    lua_setmetatable(lua_state, -2);
    lua_pop(lua_state, 1);
}

auto LuaDB::ReportError(const std::string &actor_name, const luabridge::LuaException &e) -> void {
    auto error_message = std::string(e.what());
    std::replace(error_message.begin(), error_message.end(), '\\', '/');
    std::cout << "\033[31m" << actor_name << " : " << error_message << "\033[0m" << std::endl;
}

auto LuaDB::Log(const char *message) -> void {
    std::cout << (message != nullptr ? message : "") << std::endl;
}

auto LuaDB::LogError(const char *message) -> void {
    std::cerr << (message != nullptr ? message : "") << std::endl;
}
