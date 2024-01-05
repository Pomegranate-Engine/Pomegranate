#ifndef POMEGRANATE_ENGINE_INPUT_MANAGER_H
#define POMEGRANATE_ENGINE_INPUT_MANAGER_H
#include "SDL.h"
#include "vec_math.h"


class InputManager
{
private:
    /* data */
    static bool* keys;
    static bool* mouse_buttons;
    static vec2 mouse_position;
    static vec2 mouse_delta;
    static bool mouse_moved;
    static bool mouse_scrolled;
    static vec2 mouse_scroll;
public:
    InputManager();
    ~InputManager();
    static void init();
    static void update();
    static bool get_key(int);
    static bool get_mouse_button(int);
    static vec2 get_mouse_position();
    static vec2 get_mouse_delta();
    static bool get_mouse_moved();
    static bool get_mouse_scrolled();
    static vec2 get_mouse_scroll();
};

#endif //POMEGRANATE_ENGINE_INPUT_MANAGER_H
