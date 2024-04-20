#pragma once

#include <algorithm>
#include <deque>
#include <set>
#include <unordered_map>
#include <vector>

#include "glm/glm.hpp"

#include "Actor.h"

class ActorCmp {
  public:
    bool operator()(const Actor *a, const Actor *b) const {
        return a->id < b->id;
    }
};

class ComponentQueueCmp {
  public:
    bool operator()(const Component *a, const Component *b) const {
        if (a->actor_id != b->actor_id) {
            return a->actor_id < b->actor_id;
        }
        return a->key < b->key;
    }
};

class Scene {
  public:
    static inline auto actor_id_counter = size_t{0};

    std::string name;
    std::deque<Actor> actor_store;
    std::set<Actor *, ActorCmp> actors;
    std::unordered_map<size_t, Actor *> id_to_actors;
    std::unordered_map<std::string, std::vector<Actor *>> name_to_actors;
    std::vector<Component *> start_queue;
    std::set<Component *, ComponentQueueCmp> update_queue;
    std::set<Component *, ComponentQueueCmp> late_update_queue;
    std::set<Component *, ComponentQueueCmp> destroy_queue;
    std::set<Component *, ComponentQueueCmp> has_destroy;
    std::vector<Actor *> add_actor_queue;
    std::vector<Actor *> remove_actor_queue;

    inline auto RegisterComponent(Component &component) {
        if (component.hasStart && !id_to_actors[component.actor_id]->persistent) {
            start_queue.push_back(&component);
        }
        if (component.hasUpdate) {
            update_queue.insert(&component);
        }
        if (component.hasLateUpdate) {
            late_update_queue.insert(&component);
        }
        if (component.hasDestroy) {
            has_destroy.insert(&component);
        }
    }

    inline auto UnregisterComponent(Component *component) {
        if (component->hasStart) {
            const auto it = std::find(start_queue.begin(), start_queue.end(), component);
            if (it != start_queue.end()) {
                start_queue.erase(it);
            }
        }
        if (component->hasUpdate) {
            update_queue.erase(component);
        }
        if (component->hasLateUpdate) {
            late_update_queue.erase(component);
        }
        if (component->hasDestroy) {
            destroy_queue.erase(component);
        }
    }

    inline auto RegisterActor(Actor &actor) {
        id_to_actors[actor.id] = &actor;
        name_to_actors[actor.actor_name].push_back(&actor);
    }

    inline auto UnregisterActor(Actor *actor) {
        id_to_actors.erase(actor->id);
        auto &name_to_actors_vec = name_to_actors[actor->actor_name];
        name_to_actors_vec.erase(std::find(name_to_actors_vec.begin(), name_to_actors_vec.end(), actor));
    }

    inline auto Reset() {
        for (auto &actor : actor_store) {
            actor.id = actor_id_counter++;
            actor.BuildDataStructures();
            actors.insert(&actor);
            RegisterActor(actor);
        }
        for (auto &actor : actors) {
            for (auto &[key, component] : actor->components) {
                component.actor_id = actor->id;
                RegisterComponent(component);
            }
        }
    }
};
