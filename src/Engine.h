#pragma once

#include <array>
#include <optional>
#include <queue>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "glm/glm.hpp"
#include "SDL.h"

#include "Actor.h"
#include "Config.h"
#include "Scene.h"

class Engine {
  public:
    static auto GameLoop() -> void;

    // Lua API
    // Actor
    static auto FindActor(const char *) -> luabridge::LuaRef;
    static auto FindAllActors(const char *) -> luabridge::LuaRef;
    static auto InstantiateActor(const char *) -> luabridge::LuaRef;
    static auto DestroyActor(Actor *) -> void;

    // Application
    static auto GetFrame() -> int;
    static auto Quit() -> void;
    static auto Sleep(int) -> void;
    static auto OpenURL(const char *) -> void;

    // Camera
    static auto SetCameraPosition(float, float) -> void;
    static auto GetCameraPositionX() -> float;
    static auto GetCameraPositionY() -> float;
    static auto SetCameraZoom(float) -> void;
    static auto GetCameraZoom() -> float;

    // Scene
    static auto LoadScene(const char *) -> void;
    static auto GetCurrentScene() -> std::string;
    static auto DontDestroy(Actor *) -> void;

    static inline Config config;
    static inline Scene scene;

    static inline SDL_Renderer *renderer;
    static inline glm::vec2 camera_position;

  private:
    // Core game
    static auto GameInit() -> void;
    static auto EarlyUpdate() -> void;
    static auto Input() -> void;
    static auto Update() -> void;
    static auto Render() -> void;
    static auto LateUpdate() -> void;

    // Updaters
    static auto UpdateActors() -> void;
    static auto LateUpdateActors() -> void;

    // Renderers

    // Helpers
    static auto FinishAddingActors() -> void;
    static auto FinishRemovingActors() -> void;

    static inline bool running = true;
    static inline SDL_Window *window;
    static inline std::optional<std::string> next_scene;
    static inline int frame_number = 0;
    static inline Uint32 current_frame_start_timestamp = 0;
};
