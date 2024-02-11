#ifndef POMEGRANATE_ENGINE_WINDOW_H
#define POMEGRANATE_ENGINE_WINDOW_H
#include <SDL.h>
#include "engine.h"
namespace Pomegranate
{
    class Window
    {
    private:
        /* data */
        int width, height;
        const char* title;
        SDL_Window* window;
        SDL_Renderer* renderer;
    public:

        Window(const char* title,int width, int height);
        ~Window();
        int get_width() const;
        int get_height() const;
        const char* get_title() const;
        void set_title(const char* title);
        void set_size(int width, int height);
        void set_icon(const char* path);
        SDL_Window* get_sdl_window() const;
        SDL_Renderer* get_sdl_renderer() const;
        int open();
        void close();
        void make_current();
        static const Window* current;
    };
}

#endif //POMEGRANATE_ENGINE_WINDOW_H
