#pragma once

#include <unordered_map>
#include <unordered_set>

#include "glm/glm.hpp"
#include "SDL.h"

class Input {
public:
    static auto Init() -> void; // Call before main loop begins.
    static auto ProcessEvent(const SDL_Event &) -> void; // Call every frame at start of event loop.
    static auto LateUpdate() -> void;

    static auto GetKey(const char *) -> bool;
    static auto GetKeyDown(const char *) -> bool;
    static auto GetKeyUp(const char *) -> bool;

    static auto GetMousePosition() -> glm::vec2;

    static auto GetMouseButton(int) -> bool;
    static auto GetMouseButtonDown(int) -> bool;
    static auto GetMouseButtonUp(int) -> bool;
    static auto GetMouseScrollDelta() -> float;

private:
    enum InputState {
        INPUT_DOWN, INPUT_JUST_DOWN, INPUT_UP, INPUT_JUST_UP
    };

    static inline std::unordered_map<SDL_Scancode, InputState> keyboard_states;
    static inline std::unordered_set<SDL_Scancode> just_became_down_scancodes;
    static inline std::unordered_set<SDL_Scancode> just_became_up_scancodes;

    static inline glm::vec2 mouse_position = {0, 0};
    static inline std::unordered_map<int, InputState> mouse_button_states;
    static inline std::unordered_set<int> just_became_down_mouse_buttons;
    static inline std::unordered_set<int> just_became_up_mouse_buttons;

    static inline float mouse_scroll_this_frame = 0;
};
