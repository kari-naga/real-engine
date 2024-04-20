#pragma once

#include <memory>
#include <set>

#include "box2d/box2d.h"

#include "Actor.h"
#include "LuaDB.h"

class RaycastCallback : public b2RayCastCallback {
  public:
    auto ReportFixture(b2Fixture *, const b2Vec2 &, const b2Vec2 &, float) -> float override;
};

class RaycastHitResult {
  public:
    Actor *actor;
    b2Vec2 point;
    b2Vec2 normal;
    bool is_trigger;
    
    auto operator<(const RaycastHitResult &) const -> bool;
};

class Physics {
  public:
    static inline std::set<RaycastHitResult> raycast_hits;

    static inline b2Vec2 raycast_start_point;

    static auto Init() -> void;

    static auto Step() -> void;

    static auto GetWorld() -> b2World &;

    static auto Raycast(b2Vec2, b2Vec2, float) -> luabridge::LuaRef;

    static auto RaycastAll(b2Vec2, b2Vec2, float) -> luabridge::LuaRef;

  private:
    static inline std::unique_ptr<b2World> world;

    static inline RaycastCallback raycast_callback;
};