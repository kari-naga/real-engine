#include "Input.h"

#include <iostream>

static const std::unordered_map<std::string, SDL_Scancode> __keycode_to_scancode = {
    // Directional (arrow) Keys
    {"up", SDL_SCANCODE_UP},
    {"down", SDL_SCANCODE_DOWN},
    {"right", SDL_SCANCODE_RIGHT},
    {"left", SDL_SCANCODE_LEFT},

    // Misc Keys
    {"escape", SDL_SCANCODE_ESCAPE},

    // Modifier Keys
    {"lshift", SDL_SCANCODE_LSHIFT},
    {"rshift", SDL_SCANCODE_RSHIFT},
    {"lctrl", SDL_SCANCODE_LCTRL},
    {"rctrl", SDL_SCANCODE_RCTRL},
    {"lalt", SDL_SCANCODE_LALT},
    {"ralt", SDL_SCANCODE_RALT},

    // Editing Keys
    {"tab", SDL_SCANCODE_TAB},
    {"return", SDL_SCANCODE_RETURN},
    {"enter", SDL_SCANCODE_RETURN},
    {"backspace", SDL_SCANCODE_BACKSPACE},
    {"delete", SDL_SCANCODE_DELETE},
    {"insert", SDL_SCANCODE_INSERT},

    // Character Keys
    {"space", SDL_SCANCODE_SPACE},
    {"a", SDL_SCANCODE_A},
    {"b", SDL_SCANCODE_B},
    {"c", SDL_SCANCODE_C},
    {"d", SDL_SCANCODE_D},
    {"e", SDL_SCANCODE_E},
    {"f", SDL_SCANCODE_F},
    {"g", SDL_SCANCODE_G},
    {"h", SDL_SCANCODE_H},
    {"i", SDL_SCANCODE_I},
    {"j", SDL_SCANCODE_J},
    {"k", SDL_SCANCODE_K},
    {"l", SDL_SCANCODE_L},
    {"m", SDL_SCANCODE_M},
    {"n", SDL_SCANCODE_N},
    {"o", SDL_SCANCODE_O},
    {"p", SDL_SCANCODE_P},
    {"q", SDL_SCANCODE_Q},
    {"r", SDL_SCANCODE_R},
    {"s", SDL_SCANCODE_S},
    {"t", SDL_SCANCODE_T},
    {"u", SDL_SCANCODE_U},
    {"v", SDL_SCANCODE_V},
    {"w", SDL_SCANCODE_W},
    {"x", SDL_SCANCODE_X},
    {"y", SDL_SCANCODE_Y},
    {"z", SDL_SCANCODE_Z},
    {"0", SDL_SCANCODE_0},
    {"1", SDL_SCANCODE_1},
    {"2", SDL_SCANCODE_2},
    {"3", SDL_SCANCODE_3},
    {"4", SDL_SCANCODE_4},
    {"5", SDL_SCANCODE_5},
    {"6", SDL_SCANCODE_6},
    {"7", SDL_SCANCODE_7},
    {"8", SDL_SCANCODE_8},
    {"9", SDL_SCANCODE_9},
    {"/", SDL_SCANCODE_SLASH},
    {";", SDL_SCANCODE_SEMICOLON},
    {"=", SDL_SCANCODE_EQUALS},
    {"-", SDL_SCANCODE_MINUS},
    {".", SDL_SCANCODE_PERIOD},
    {",", SDL_SCANCODE_COMMA},
    {"[", SDL_SCANCODE_LEFTBRACKET},
    {"]", SDL_SCANCODE_RIGHTBRACKET},
    {"\\", SDL_SCANCODE_BACKSLASH},
    {"'", SDL_SCANCODE_APOSTROPHE}};

auto Input::Init() -> void {
    for (auto i = 0U; i < SDL_NUM_SCANCODES; ++i) {
        keyboard_states[static_cast<SDL_Scancode>(i)] = INPUT_UP;
    }
    for (auto i = 0U; i < UINT8_MAX; ++i) {
        mouse_button_states[i] = INPUT_UP;
    }
}

auto Input::ProcessEvent(const SDL_Event &e) -> void {
    switch (e.type) {
    case SDL_KEYDOWN:
        keyboard_states[e.key.keysym.scancode] = INPUT_JUST_DOWN;
        just_became_up_scancodes.erase(e.key.keysym.scancode);
        just_became_down_scancodes.insert(e.key.keysym.scancode);
        break;
    case SDL_KEYUP:
        keyboard_states[e.key.keysym.scancode] = INPUT_JUST_UP;
        just_became_down_scancodes.erase(e.key.keysym.scancode);
        just_became_up_scancodes.insert(e.key.keysym.scancode);
        break;
    case SDL_MOUSEBUTTONDOWN:
        mouse_button_states[e.button.button] = INPUT_JUST_DOWN;
        just_became_up_mouse_buttons.erase(e.button.button);
        just_became_down_mouse_buttons.insert(e.button.button);
        break;
    case SDL_MOUSEBUTTONUP:
        mouse_button_states[e.button.button] = INPUT_JUST_UP;
        just_became_down_mouse_buttons.erase(e.button.button);
        just_became_up_mouse_buttons.insert(e.button.button);
        break;
    case SDL_MOUSEMOTION:
        mouse_position.x = static_cast<float>(e.motion.x);
        mouse_position.y = static_cast<float>(e.motion.y);
        break;
    case SDL_MOUSEWHEEL:
        mouse_scroll_this_frame = e.wheel.preciseY;
        break;
    }
}

auto Input::LateUpdate() -> void {
    for (const auto scancode : just_became_down_scancodes) {
        keyboard_states[scancode] = INPUT_DOWN;
    }
    just_became_down_scancodes.clear();
    for (const auto scancode : just_became_up_scancodes) {
        keyboard_states[scancode] = INPUT_UP;
    }
    just_became_up_scancodes.clear();
    for (const auto mousebutton : just_became_down_mouse_buttons) {
        mouse_button_states[mousebutton] = INPUT_DOWN;
    }
    just_became_down_mouse_buttons.clear();
    for (const auto mousebutton : just_became_up_mouse_buttons) {
        mouse_button_states[mousebutton] = INPUT_UP;
    }
    just_became_up_mouse_buttons.clear();
    mouse_scroll_this_frame = 0;
}

auto Input::GetKey(const char *keycode) -> bool {
    if (const auto scancode_it = __keycode_to_scancode.find(keycode); scancode_it != __keycode_to_scancode.end()) {
        const auto scancode = scancode_it->second;
        if (const auto it = keyboard_states.find(scancode); it != keyboard_states.end()) {
            if (it->second == INPUT_DOWN || it->second == INPUT_JUST_DOWN) {
                return true;
            }
        }
    }
    return false;
}

auto Input::GetKeyDown(const char *keycode) -> bool {
    if (const auto scancode_it = __keycode_to_scancode.find(keycode); scancode_it != __keycode_to_scancode.end()) {
        const auto scancode = scancode_it->second;
        if (const auto it = keyboard_states.find(scancode); it != keyboard_states.end()) {
            if (it->second == INPUT_JUST_DOWN) {
                return true;
            }
        }
    }
    return false;
}

auto Input::GetKeyUp(const char *keycode) -> bool {
    if (const auto scancode_it = __keycode_to_scancode.find(keycode); scancode_it != __keycode_to_scancode.end()) {
        const auto scancode = scancode_it->second;
        if (const auto it = keyboard_states.find(scancode); it != keyboard_states.end()) {
            if (it->second == INPUT_JUST_UP) {
                return true;
            }
        }
    }
    return false;
}

auto Input::GetMousePosition() -> glm::vec2 {
    return mouse_position;
}

auto Input::GetMouseButton(int mousebutton) -> bool {
    if (const auto it = mouse_button_states.find(mousebutton); it != mouse_button_states.end()) {
        if (it->second == INPUT_DOWN || it->second == INPUT_JUST_DOWN) {
            return true;
        }
    }
    return false;
}

auto Input::GetMouseButtonDown(int mousebutton) -> bool {
    if (const auto it = mouse_button_states.find(mousebutton); it != mouse_button_states.end()) {
        if (it->second == INPUT_JUST_DOWN) {
            return true;
        }
    }
    return false;
}

auto Input::GetMouseButtonUp(int mousebutton) -> bool {
    if (const auto it = mouse_button_states.find(mousebutton); it != mouse_button_states.end()) {
        if (it->second == INPUT_JUST_UP) {
            return true;
        }
    }
    return false;
}

auto Input::GetMouseScrollDelta() -> float {
    return mouse_scroll_this_frame;
}
