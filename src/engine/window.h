#ifndef POMEGRANATE_ENGINE_WINDOW_H
#define POMEGRANATE_ENGINE_WINDOW_H
#include <SDL.h>
#include "engine.h"

class Window
{
private:
    /* data */
    unsigned int width, height;
    const char* title;
    SDL_Window* window;
    SDL_Renderer* renderer;
public:

    Window(const char* title,int width, int height);
    ~Window();
    unsigned int get_width() const;
    unsigned int get_height() const;
    const char* get_title() const;
    SDL_Window* get_sdl_window() const;
    SDL_Renderer* get_sdl_renderer() const;
    int open();
    void close();
    void make_current();
    static Window* current;
};

#endif //POMEGRANATE_ENGINE_WINDOW_H
