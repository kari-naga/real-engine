#include "Physics.h"

#include "Rigidbody.h"
#include "Time.h"

auto RaycastCallback::ReportFixture(b2Fixture *fixture, const b2Vec2 &point, const b2Vec2 &normal, float fraction) -> float {
    const auto category = fixture->GetFilterData().categoryBits;
    if (category == RB_PHANTOM) {
        return -1;
    }
    auto hit = RaycastHitResult{};
    hit.actor = reinterpret_cast<Actor *>(fixture->GetUserData().pointer);
    hit.point = point;
    hit.normal = normal;
    hit.is_trigger = category == RB_TRIGGER;
    Physics::raycast_hits.insert(hit);
    return 1;
}

auto RaycastHitResult::operator<(const RaycastHitResult &other) const -> bool {
    auto dist_to_start = point.operator_sub(Physics::raycast_start_point).Length();
    auto other_dist_to_start = other.point.operator_sub(Physics::raycast_start_point).Length();
    return dist_to_start < other_dist_to_start;
}

auto Physics::Init() -> void {
    world = std::make_unique<b2World>(b2Vec2(0.0f, 9.8f));
}

auto Physics::Step() -> void {
    if (world) {
        world->Step(Time::DeltaTime(), 8, 3);
    }
}

auto Physics::GetWorld() -> b2World & {
    if (!world) {
        Init();
    }
    return *world;
}

auto Physics::Raycast(b2Vec2 pos, b2Vec2 dir, float dist) -> luabridge::LuaRef {
    raycast_hits.clear();
    raycast_start_point = pos;
    dir.Normalize();
    GetWorld().RayCast(&raycast_callback, pos, pos + dir.operator_mul(dist));
    const auto lua_state = LuaDB::GetLuaState();
    if (raycast_hits.empty()) {
        return luabridge::LuaRef(lua_state);
    }
    return luabridge::LuaRef(lua_state, &*raycast_hits.begin());
}

auto Physics::RaycastAll(b2Vec2 pos, b2Vec2 dir, float dist) -> luabridge::LuaRef {
    raycast_hits.clear();
    raycast_start_point = pos;
    dir.Normalize();
    GetWorld().RayCast(&raycast_callback, pos, pos + dir.operator_mul(dist));
    const auto hits = luabridge::newTable(LuaDB::GetLuaState());
    auto i = 1;
    for (const auto &hit : raycast_hits) {
        hits[i] = &hit;
        ++i;
    }
    return hits;
}
