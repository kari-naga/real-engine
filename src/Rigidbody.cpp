#include "Rigidbody.h"

#include <memory>

#include "glm/glm.hpp"

#include "EngineUtils.h"
#include "Physics.h"
#include "Engine.h"

auto RigidbodyContactListener::BeginContact(b2Contact *contact) -> void {
    const auto fixtureA = contact->GetFixtureA();
    const auto actorA = reinterpret_cast<Actor *>(fixtureA->GetUserData().pointer);
    const auto fixtureB = contact->GetFixtureB();
    const auto actorB = reinterpret_cast<Actor *>(fixtureB->GetUserData().pointer);
    auto manifold = b2WorldManifold{};
    contact->GetWorldManifold(&manifold);
    auto collisionA = RigidbodyCollision{};
    collisionA.other = actorB;
    collisionA.relative_velocity = fixtureA->GetBody()->GetLinearVelocity() - fixtureB->GetBody()->GetLinearVelocity();
    const auto categoryA = fixtureA->GetFilterData().categoryBits;
    if (categoryA == RB_COLLIDER) {
        collisionA.point = manifold.points[0];
        collisionA.normal = manifold.normal;
        for (const auto &[key, component] : actorA->collision_enter_components) {
            if (component->IsEnabled()) {
                try {
                    (*component->ref)["OnCollisionEnter"](*component->ref, &collisionA);
                } catch (luabridge::LuaException const &e) {
                    LuaDB::ReportError(Engine::scene.id_to_actors[component->actor_id]->actor_name, e);
                }
            }
        }
    } else if (categoryA == RB_TRIGGER) {
        collisionA.point = {-999.0f, -999.0f};
        collisionA.normal = {-999.0f, -999.0f};
        for (const auto &[key, component] : actorA->trigger_enter_components) {
            if (component->IsEnabled()) {
                try {
                    (*component->ref)["OnTriggerEnter"](*component->ref, &collisionA);
                } catch (luabridge::LuaException const &e) {
                    LuaDB::ReportError(Engine::scene.id_to_actors[component->actor_id]->actor_name, e);
                }
            }
        }
    }
    auto collisionB = RigidbodyCollision{};
    collisionB.other = actorA;
    collisionB.relative_velocity = collisionA.relative_velocity;
    const auto categoryB = fixtureB->GetFilterData().categoryBits;
    if (categoryB == RB_COLLIDER) {
        collisionB.point = manifold.points[0];
        collisionB.normal = manifold.normal;
        for (const auto &[key, component] : actorB->collision_enter_components) {
            if (component->IsEnabled()) {
                try {
                    (*component->ref)["OnCollisionEnter"](*component->ref, &collisionB);
                } catch (luabridge::LuaException const &e) {
                    LuaDB::ReportError(Engine::scene.id_to_actors[component->actor_id]->actor_name, e);
                }
            }
        }
    } else if (categoryB == RB_TRIGGER) {
        collisionB.point = {-999.0f, -999.0f};
        collisionB.normal = {-999.0f, -999.0f};
        for (const auto &[key, component] : actorB->trigger_enter_components) {
            if (component->IsEnabled()) {
                try {
                    (*component->ref)["OnTriggerEnter"](*component->ref, &collisionB);
                } catch (luabridge::LuaException const &e) {
                    LuaDB::ReportError(Engine::scene.id_to_actors[component->actor_id]->actor_name, e);
                }
            }
        }
    }
}

auto RigidbodyContactListener::EndContact(b2Contact *contact) -> void {
    const auto fixtureA = contact->GetFixtureA();
    const auto actorA = reinterpret_cast<Actor *>(fixtureA->GetUserData().pointer);
    const auto fixtureB = contact->GetFixtureB();
    const auto actorB = reinterpret_cast<Actor *>(fixtureB->GetUserData().pointer);
    auto manifold = b2WorldManifold{};
    contact->GetWorldManifold(&manifold);
    auto collisionA = RigidbodyCollision{};
    collisionA.other = actorB;
    collisionA.relative_velocity = fixtureA->GetBody()->GetLinearVelocity() - fixtureB->GetBody()->GetLinearVelocity();
    collisionA.point = {-999.0f, -999.0f};
    collisionA.normal = {-999.0f, -999.0f};
    const auto categoryA = fixtureA->GetFilterData().categoryBits;
    if (categoryA == RB_COLLIDER) {
        for (const auto &[key, component] : actorA->collision_exit_components) {
            if (component->IsEnabled()) {
                try {
                    (*component->ref)["OnCollisionExit"](*component->ref, &collisionA);
                } catch (luabridge::LuaException const &e) {
                    LuaDB::ReportError(Engine::scene.id_to_actors[component->actor_id]->actor_name, e);
                }
            }
        }
    } else if (categoryA == RB_TRIGGER) {
        for (const auto &[key, component] : actorA->trigger_exit_components) {
            if (component->IsEnabled()) {
                try {
                    (*component->ref)["OnTriggerExit"](*component->ref, &collisionA);
                } catch (luabridge::LuaException const &e) {
                    LuaDB::ReportError(Engine::scene.id_to_actors[component->actor_id]->actor_name, e);
                }
            }
        }
    }
    auto collisionB = RigidbodyCollision{};
    collisionB.other = actorA;
    collisionB.relative_velocity = collisionA.relative_velocity;
    collisionB.point = {-999.0f, -999.0f};
    collisionB.normal = {-999.0f, -999.0f};
    const auto categoryB = fixtureB->GetFilterData().categoryBits;
    if (categoryB == RB_COLLIDER) {
        for (const auto &[key, component] : actorB->collision_exit_components) {
            if (component->IsEnabled()) {
                try {
                    (*component->ref)["OnCollisionExit"](*component->ref, &collisionB);
                } catch (luabridge::LuaException const &e) {
                    LuaDB::ReportError(Engine::scene.id_to_actors[component->actor_id]->actor_name, e);
                }
            }
        }
    } else if (categoryB == RB_TRIGGER) {
        for (const auto &[key, component] : actorB->trigger_exit_components) {
            if (component->IsEnabled()) {
                try {
                    (*component->ref)["OnTriggerExit"](*component->ref, &collisionB);
                } catch (luabridge::LuaException const &e) {
                    LuaDB::ReportError(Engine::scene.id_to_actors[component->actor_id]->actor_name, e);
                }
            }
        }
    }
}

auto Rigidbody::GetPosition() -> b2Vec2 {
    if (body == nullptr) {
        return {x, y};
    }
    return body->GetPosition();
}

auto Rigidbody::GetRotation() -> float {
    if (body == nullptr) {
        return rotation;
    }
    return EngineUtils::RadToDeg(body->GetAngle());
}

auto Rigidbody::GetVelocity() -> b2Vec2 {
    return body->GetLinearVelocity();
}

auto Rigidbody::GetAngularVelocity() -> float {
    return EngineUtils::RadToDeg(body->GetAngularVelocity());
}

auto Rigidbody::GetGravityScale() -> float {
    if (body == nullptr) {
        return gravity_scale;
    }
    return body->GetGravityScale();
}

auto Rigidbody::GetUpDirection() -> b2Vec2 {
    float angle;
    if (body == nullptr) {
        angle = EngineUtils::DegToRad(rotation);
    } else {
        angle = body->GetAngle();
    }
    auto result = b2Vec2(glm::sin(angle), -glm::cos(angle));
    result.Normalize();
    return result;
}

auto Rigidbody::GetRightDirection() -> b2Vec2 {
    float angle;
    if (body == nullptr) {
        angle = EngineUtils::DegToRad(rotation);
    } else {
        angle = body->GetAngle();
    }
    auto result = b2Vec2(glm::cos(angle), glm::sin(angle));
    result.Normalize();
    return result;
}

auto Rigidbody::AddForce(b2Vec2 force) -> void {
    body->ApplyForceToCenter(force, true);
}

auto Rigidbody::AddTorque(float torque) -> void {
    body->ApplyTorque(torque, true);
}

auto Rigidbody::AddLinearImpulse(b2Vec2 impulse) -> void {
    body->ApplyLinearImpulseToCenter(impulse, true);
}

auto Rigidbody::AddAngularImpulse(float impulse) -> void {
    body->ApplyAngularImpulse(impulse, true);
}

auto Rigidbody::SetVelocity(b2Vec2 vel) -> void {
    body->SetLinearVelocity(vel);
}

auto Rigidbody::SetPosition(b2Vec2 pos) -> void {
    if (body == nullptr) {
        x = pos.x;
        y = pos.y;
        return;
    }
    body->SetTransform(pos, body->GetAngle());
}

auto Rigidbody::SetRotation(float degrees_clockwise) -> void {
    if (body == nullptr) {
        rotation = degrees_clockwise;
        return;
    }
    body->SetTransform(body->GetPosition(), EngineUtils::DegToRad(degrees_clockwise));
}

auto Rigidbody::SetAngularVelocity(float ang_vel) -> void {
    body->SetAngularVelocity(EngineUtils::DegToRad(ang_vel));
}

auto Rigidbody::SetGravityScale(float scale) -> void {
    if (body == nullptr) {
        gravity_scale = scale;
        return;
    }
    body->SetGravityScale(scale);
}

auto Rigidbody::SetUpDirection(b2Vec2 direction) -> void {
    direction.Normalize();
    const auto angle = glm::atan(direction.x, -direction.y);
    if (body == nullptr) {
        rotation = angle;
        return;
    }
    body->SetTransform(body->GetPosition(), angle);
}

auto Rigidbody::SetRightDirection(b2Vec2 direction) -> void {
    direction.Normalize();
    const auto angle = glm::atan(direction.x, -direction.y) - b2_pi / 2.0f;
    if (body == nullptr) {
        rotation = angle;
        return;
    }
    body->SetTransform(body->GetPosition(), angle);
}

auto Rigidbody::OnStart() -> void {
    auto def = b2BodyDef{};
    if (body_type == "dynamic") {
        def.type = b2_dynamicBody;
    } else if (body_type == "static") {
        def.type = b2_staticBody;
    } else if (body_type == "kinematic") {
        def.type = b2_kinematicBody;
    } else {
        std::cout << "error: bad rigidbody type given";
        exit(0);
    }
    def.bullet = precise;
    def.angularDamping = angular_friction;
    def.gravityScale = gravity_scale;
    def.position.Set(x, y);
    def.angle = EngineUtils::DegToRad(rotation);
    auto &world = Physics::GetWorld();
    body = world.CreateBody(&def);
    world.SetContactListener(&contact_listener);
    if (!has_collider && !has_trigger) {
        auto phantom_shape = b2PolygonShape{};
        phantom_shape.SetAsBox(width * 0.5f, height * 0.5f);
        auto phantom_fixture_def = b2FixtureDef{};
        phantom_fixture_def.shape = &phantom_shape;
        phantom_fixture_def.density = density;
        phantom_fixture_def.isSensor = true;
        phantom_fixture_def.filter.categoryBits = RB_PHANTOM;
        phantom_fixture_def.filter.maskBits = 0;
        body->CreateFixture(&phantom_fixture_def);
    } else {
        if (has_collider) {
            auto collider_shape = std::unique_ptr<b2Shape>();
            if (collider_type == "box") {
                collider_shape = std::make_unique<b2PolygonShape>();
                static_cast<b2PolygonShape *>(collider_shape.get())->SetAsBox(width * 0.5f, height * 0.5f);
            } else if (collider_type == "circle") {
                collider_shape = std::make_unique<b2CircleShape>();
                collider_shape->m_radius = radius;
            } else {
                std::cout << "error: bad collider type given";
                exit(0);
            }
            auto collider_fixture_def = b2FixtureDef{};
            collider_fixture_def.shape = collider_shape.get();
            collider_fixture_def.density = density;
            collider_fixture_def.isSensor = false;
            collider_fixture_def.friction = friction;
            collider_fixture_def.restitution = bounciness;
            collider_fixture_def.filter.categoryBits = RB_COLLIDER;
            collider_fixture_def.filter.maskBits = RB_COLLIDER;
            collider_fixture_def.userData.pointer = reinterpret_cast<uintptr_t>(actor);
            body->CreateFixture(&collider_fixture_def);
        }
        if (has_trigger) {
            auto trigger_shape = std::unique_ptr<b2Shape>();
            if (trigger_type == "box") {
                trigger_shape = std::make_unique<b2PolygonShape>();
                static_cast<b2PolygonShape *>(trigger_shape.get())->SetAsBox(trigger_width * 0.5f, trigger_height * 0.5f);
            } else if (trigger_type == "circle") {
                trigger_shape = std::make_unique<b2CircleShape>();
                trigger_shape->m_radius = trigger_radius;
            } else {
                std::cout << "error: bad trigger type given";
                exit(0);
            }
            auto trigger_fixture_def = b2FixtureDef{};
            trigger_fixture_def.shape = trigger_shape.get();
            trigger_fixture_def.density = density;
            trigger_fixture_def.isSensor = true;
            trigger_fixture_def.filter.categoryBits = RB_TRIGGER;
            trigger_fixture_def.filter.maskBits = RB_TRIGGER;
            trigger_fixture_def.userData.pointer = reinterpret_cast<uintptr_t>(actor);
            body->CreateFixture(&trigger_fixture_def);
        }
    }
}

auto Rigidbody::OnDestroy() -> void {
    Physics::GetWorld().DestroyBody(body);
}

static std::deque<Rigidbody> rb_storage;

auto Rigidbody::MakeRigidbody() -> Rigidbody * {
    rb_storage.push_back(Rigidbody{});
    return &rb_storage.back();
}
