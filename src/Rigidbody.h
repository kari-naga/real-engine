#pragma once

#include <string>
#include <deque>

#include "box2d/box2d.h"

#include "Actor.h"

class RigidbodyContactListener : public b2ContactListener {
  public:
    auto BeginContact(b2Contact *) -> void override;
    auto EndContact(b2Contact *) -> void override;
};

enum RigidbodyFixtureCategory {
    RB_PHANTOM = 0x1,
    RB_COLLIDER = 0x2,
    RB_TRIGGER = 0x4,
};

class RigidbodyCollision {
  public:
    Actor *other;
    b2Vec2 point;
    b2Vec2 relative_velocity;
    b2Vec2 normal;
};

class Rigidbody {
  public:
    float x = 0.0f;
    float y = 0.0f;
    std::string body_type = "dynamic";
    bool precise = false;
    float gravity_scale = 1.0f;
    float density = 1.0f;
    float angular_friction = 0.3f;
    float rotation = 0.0f;
    bool has_collider = true;
    bool has_trigger = false;
    std::string collider_type = "box";
    float width = 1.0f;
    float height = 1.0f;
    float radius = 0.5f;
    float friction = 0.3f;
    float bounciness = 0.3f;
    std::string trigger_type = "box";
    float trigger_width = 1.0f;
    float trigger_height = 1.0f;
    float trigger_radius = 0.5f;

    std::string type = "Rigidbody";
    std::string key;
    Actor *actor = nullptr;
    bool enabled = true;

    auto GetPosition() -> b2Vec2;

    auto GetRotation() -> float;

    auto GetVelocity() -> b2Vec2;

    auto GetAngularVelocity() -> float;

    auto GetGravityScale() -> float;

    auto GetUpDirection() -> b2Vec2;

    auto GetRightDirection() -> b2Vec2;

    auto AddForce(b2Vec2) -> void;
    
    auto AddTorque(float) -> void;

    auto AddLinearImpulse(b2Vec2) -> void;

    auto AddAngularImpulse(float) -> void;

    auto SetVelocity(b2Vec2) -> void;

    auto SetPosition(b2Vec2) -> void;

    auto SetRotation(float) -> void;

    auto SetAngularVelocity(float) -> void;

    auto SetGravityScale(float) -> void;

    auto SetUpDirection(b2Vec2) -> void;

    auto SetRightDirection(b2Vec2) -> void;

    auto OnStart() -> void;

    auto OnDestroy() -> void;

    static auto MakeRigidbody() -> Rigidbody *;

  private:
    b2Body *body;
    RigidbodyContactListener contact_listener;
};