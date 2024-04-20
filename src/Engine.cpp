#include "Engine.h"

#include <algorithm>
#include <cmath>
#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <optional>
#include <string>
#include <thread>
#include <tuple>
#include <utility>

#include "SDL_mixer.h"
#include "SDL_image.h"
#include "SDL.h"

#include "AudioDB.h"
#include "EngineUtils.h"
#include "Event.h"
#include "Input.h"
#include "LuaDB.h"
#include "SceneDB.h"
#include "TextDB.h"
#include "Time.h"
#include "TextureDB.h"
#include "Physics.h"

/***************
 * Core game
 ***************/

auto Engine::GameLoop() -> void {
    GameInit();

    while (running) {
        EarlyUpdate();

        Input();

        Update();

        Render();

        LateUpdate();
    }
}

auto Engine::GameInit() -> void {
    if (!std::filesystem::exists("resources")) {
        std::cout << "error: resources/ missing";
        exit(0);
    } else if (!std::filesystem::exists("resources/game.config")) {
        std::cout << "error: resources/game.config missing";
        exit(0);
    }
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
    LuaDB::Init();
    Mix_OpenAudio(48000, AUDIO_F32SYS, 2, 2048);
    Mix_AllocateChannels(50);
    auto config_doc = rapidjson::Document();
    EngineUtils::ReadJsonFile("resources/game.config", config_doc);
    if (const auto it = config_doc.FindMember("initial_scene"); it == config_doc.MemberEnd() || !it->value.IsString()) {
        std::cout << "error: initial_scene unspecified";
        exit(0);
    }
    config.ParseGameConfig(config_doc);
    auto rendering_doc = rapidjson::Document();
    if (std::filesystem::exists("resources/rendering.config")) {
        EngineUtils::ReadJsonFile("resources/rendering.config", rendering_doc);
    }
    config.ParseRenderingConfig(rendering_doc);
    window = SDL_CreateWindow(config.game_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, config.window_width, config.window_height, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, config.clear_color_r, config.clear_color_g, config.clear_color_b, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    camera_position = config.initial_camera_position;
    if (config.world_name != "") {
        SceneDB::LoadWorld(config.world_name);
    }
    scene = SceneDB::LoadScene(config.initial_scene_name);
    scene.Reset();
    Input::Init();
    Time::Init();
}

auto Engine::EarlyUpdate() -> void {
    if (next_scene) {
        auto persistent = std::vector<Actor>();
        for (const auto actor : scene.actors) {
            if (actor->persistent) {
                persistent.push_back(std::move(*actor));
            }
        }
        scene = SceneDB::LoadScene(next_scene.value());
        scene.actor_store.insert(scene.actor_store.begin(), persistent.begin(), persistent.end());
        next_scene = std::nullopt;
        scene.Reset();
    }
}

auto Engine::Input() -> void {
    auto event = SDL_Event();
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            running = false;
            break;
        default:
            Input::ProcessEvent(event);
        }
    }
}

auto Engine::Update() -> void {
    Time::Tick();
    for (const auto &component : scene.start_queue) {
        if (component->IsEnabled()) {
            try {
                (*component->ref)["OnStart"](*component->ref);
            } catch (luabridge::LuaException const &e) {
                LuaDB::ReportError(scene.id_to_actors[component->actor_id]->actor_name, e);
            }
        }
    }
    scene.start_queue.clear();
    for (const auto &component : scene.update_queue) {
        if (component->IsEnabled()) {
            try {
                (*component->ref)["OnUpdate"](*component->ref);
            } catch (luabridge::LuaException const &e) {
                LuaDB::ReportError(scene.id_to_actors[component->actor_id]->actor_name, e);
            }
        }
    }
    for (const auto &component : scene.late_update_queue) {
        if (component->IsEnabled()) {
            try {
                (*component->ref)["OnLateUpdate"](*component->ref);
            } catch (luabridge::LuaException const &e) {
                LuaDB::ReportError(scene.id_to_actors[component->actor_id]->actor_name, e);
            }
        }
    }
    for (const auto &component : scene.destroy_queue) {
        try {
            (*component->ref)["OnDestroy"](*component->ref);
        } catch (luabridge::LuaException const &e) {
            LuaDB::ReportError(scene.id_to_actors[component->actor_id]->actor_name, e);
        }
    }
    Event::ResolveEvents();
    Physics::Step();
}

auto Engine::Render() -> void {
    SDL_SetRenderDrawColor(renderer, config.clear_color_r, config.clear_color_g, config.clear_color_b, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    TextureDB::RenderTiles();
    TextureDB::RenderScene();
    TextureDB::RenderUI();
    TextDB::RenderText();
    TextureDB::RenderPixels();
    static bool initialized = false;
    if (!initialized) {
        frame_number = 0;
        current_frame_start_timestamp = SDL_GetTicks();
        initialized = true;
    }
    SDL_RenderPresent(renderer);
    Uint32 current_frame_end_timestamp = SDL_GetTicks(); // Record end time of the frame
    Uint32 current_frame_duration_milliseconds = current_frame_end_timestamp - current_frame_start_timestamp;
    int delay_ticks = std::max(static_cast<int>(config.min_milliseconds_between_frames) - static_cast<int>(current_frame_duration_milliseconds), 1);
    SDL_Delay(delay_ticks);
    current_frame_start_timestamp = SDL_GetTicks();
    frame_number++;
}

auto Engine::LateUpdate() -> void {
    Input::LateUpdate();
    FinishRemovingActors();
    FinishAddingActors();
    Actor::FinishRemovingComponents();
    Actor::FinishAddingComponents();
}

/***************
 * Updaters
 ***************/

/***************
 * Renderers
 ***************/

/***************
 * Helpers
 ***************/
auto Engine::FinishAddingActors() -> void {
    for (const auto actor : scene.add_actor_queue) {
        scene.actors.insert(actor);
        for (auto &[key, component] : actor->components) {
            component.actor_id = actor->id;
            scene.RegisterComponent(component);
        }
    }
    scene.add_actor_queue.clear();
}

auto Engine::FinishRemovingActors() -> void {
    for (const auto actor : scene.remove_actor_queue) {
        for (auto &[key, component] : actor->components) {
            scene.UnregisterComponent(&component);
        }
        const auto it = std::find(scene.actors.begin(), scene.actors.end(), actor);
        if (it != scene.actors.end()) {
            scene.actors.erase(it);
        }
    }
    scene.remove_actor_queue.clear();
}

/***************
 * Lua API
 ***************/
auto Engine::FindActor(const char *name) -> luabridge::LuaRef {
    const auto lua_state = LuaDB::GetLuaState();
    if (const auto it = scene.name_to_actors.find(name); it != scene.name_to_actors.end() && !it->second.empty()) {
        return luabridge::LuaRef(lua_state, &it->second.front());
    }
    return luabridge::LuaRef(lua_state);
}

auto Engine::FindAllActors(const char *name) -> luabridge::LuaRef {
    const auto lua_state = LuaDB::GetLuaState();
    const auto found_components = luabridge::newTable(lua_state);
    if (const auto it = scene.name_to_actors.find(name); it != scene.name_to_actors.end()) {
        auto i = 1;
        for (const auto actor : it->second) {
            found_components[i] = luabridge::LuaRef(lua_state, actor);
            ++i;
        }
    }
    return found_components;
}

auto Engine::InstantiateActor(const char *template_name) -> luabridge::LuaRef {
    if (template_name == nullptr) {
        return luabridge::LuaRef(LuaDB::GetLuaState());
    }
    auto actor = TemplateDB::LoadTemplate(template_name);
    actor.id = Scene::actor_id_counter++;
    scene.actor_store.push_back(actor);
    scene.actor_store.back().BuildDataStructures();
    scene.RegisterActor(scene.actor_store.back());
    scene.add_actor_queue.push_back(&scene.actor_store.back());
    return luabridge::LuaRef(LuaDB::GetLuaState(), &scene.actor_store.back());
}

auto Engine::DestroyActor(Actor *actor) -> void {
    const auto it = std::find(scene.add_actor_queue.begin(), scene.add_actor_queue.end(), actor);
    if (it != scene.add_actor_queue.end()) {
        scene.add_actor_queue.erase(it);
    }
    for (auto &[key, component] : actor->components) {
        component.SetEnabled(false);
        if (component.hasDestroy) {
            scene.destroy_queue.insert(&component);
        }
    }
    scene.UnregisterActor(actor);
    scene.remove_actor_queue.push_back(actor); // get actor through Lua?
}

auto Engine::GetFrame() -> int {
    return frame_number;
}

auto Engine::Quit() -> void {
    exit(0);
}

auto Engine::Sleep(int duration) -> void {
    std::this_thread::sleep_for(std::chrono::milliseconds(duration));
}

auto Engine::OpenURL(const char *url) -> void {
    if (url == nullptr) {
        return;
    }
#if defined(_WIN32)
    std::system(("start " + std::string(url)).c_str());
#elif defined(__APPLE__)
    std::system(("open " + std::string(url)).c_str());
#else
    std::system(("xdg-open " + std::string(url)).c_str());
#endif
}

auto Engine::SetCameraPosition(float x, float y) -> void {
    camera_position.x = x;
    camera_position.y = y;
}

auto Engine::GetCameraPositionX() -> float {
    return camera_position.x;
}

auto Engine::GetCameraPositionY() -> float {
    return camera_position.y;
}

auto Engine::SetCameraZoom(float zoom_factor) -> void {
    config.zoom_factor = zoom_factor;
}

auto Engine::GetCameraZoom() -> float {
    return config.zoom_factor;
}

auto Engine::LoadScene(const char *scene_name) -> void {
    if (scene_name == nullptr) {
        return;
    }
    next_scene = scene_name;
    for (const auto component : scene.has_destroy) {
        scene.destroy_queue.insert(component);
    }
}

auto Engine::GetCurrentScene() -> std::string {
    return scene.name;
}

auto Engine::DontDestroy(Actor *actor) -> void {
    actor->persistent = true;
}
