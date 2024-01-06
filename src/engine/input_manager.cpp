#include "input_manager.h"

InputManager::InputManager() {

}

InputManager::~InputManager() {

}

void InputManager::update() {
    InputManager::mouse_delta = vec2();
    InputManager::mouse_moved = false;
    InputManager::mouse_scrolled = false;
    InputManager::mouse_scroll = vec2();

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_EVENT_KEY_DOWN:
                InputManager::keys[event.key.keysym.scancode] = true;
                break;
            case SDL_EVENT_KEY_UP:
                InputManager::keys[event.key.keysym.scancode] = false;
                break;
            case SDL_EVENT_MOUSE_BUTTON_DOWN:
                InputManager::mouse_buttons[event.button.button] = true;
                break;
            case SDL_EVENT_MOUSE_BUTTON_UP:
                InputManager::mouse_buttons[event.button.button] = false;
                break;
            case SDL_EVENT_MOUSE_MOTION:
                InputManager::mouse_position = vec2(event.motion.x, event.motion.y);
                InputManager::mouse_delta = vec2(event.motion.xrel, event.motion.yrel);
                InputManager::mouse_moved = true;
                break;
            case SDL_EVENT_MOUSE_WHEEL:
                InputManager::mouse_scroll = vec2(event.wheel.x, event.wheel.y);
                InputManager::mouse_scrolled = true;
                break;
            default:
                break;
        }
    }
}

bool InputManager::get_key(int key_code) {
    return InputManager::keys[key_code];
}

bool InputManager::get_mouse_button(int mouse_code) {
    return InputManager::mouse_buttons[mouse_code];
}

vec2 InputManager::get_mouse_position() {
    return InputManager::mouse_position;
}

vec2 InputManager::get_mouse_delta() {
    return InputManager::mouse_delta;
}

bool InputManager::get_mouse_moved() {
    return InputManager::mouse_moved;
}

bool InputManager::get_mouse_scrolled() {
    return InputManager::mouse_scrolled;
}

vec2 InputManager::get_mouse_scroll() {
    return InputManager::mouse_scroll;
}

void InputManager::init()
{
    InputManager::keys = new bool[SDL_NUM_SCANCODES];
    InputManager::mouse_buttons = new bool[8];
    for (int i = 0; i < SDL_NUM_SCANCODES; ++i) {
        InputManager::keys[i] = false;
    }
    for (int i = 0; i < 8; ++i) {
        InputManager::mouse_buttons[i] = false;
    }
    InputManager::mouse_position = vec2();
    InputManager::mouse_delta = vec2();
    InputManager::mouse_moved = false;
    InputManager::mouse_scrolled = false;
    InputManager::mouse_scroll = vec2();
}

bool* InputManager::keys;
bool* InputManager::mouse_buttons;
vec2 InputManager::mouse_position;
vec2 InputManager::mouse_delta;
bool InputManager::mouse_moved;
bool InputManager::mouse_scrolled;
vec2 InputManager::mouse_scroll;