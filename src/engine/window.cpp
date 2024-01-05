#include "window.h"

Window::Window(const char* title, int width, int height)
{
    this->title = title;
    this->width = width;
    this->height = height;

    this->window = nullptr;
    this->renderer = nullptr;
}

Window::~Window()
{
    this->close();
}

unsigned int Window::get_width() const
{
    return this->width;
}

unsigned int Window::get_height() const
{
    return this->height;
}

SDL_Window* Window::get_sdl_window() const
{
    return this->window;
}
SDL_Renderer* Window::get_sdl_renderer() const
{
    return this->renderer;
}

int Window::open()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        print_error("Failed to initialize the SDL2 library");
        return -1;
    }

    this->window = SDL_CreateWindow(this->title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, this->width, this->height, 0);

    if (!this->window)
    {
        std::cout << "Failed to create window\n" << std::endl;
        return -1;
    }

    this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED);

    if (!this->renderer)
    {
        SDL_DestroyWindow(this->window);
        SDL_Quit();
        print_error("Failed to create renderer");
        return -1;
    }
    return 0;
}

void Window::close()
{
    SDL_DestroyWindow(this->window);
    SDL_DestroyRenderer(this->renderer);
}

const char *Window::get_title() const
{
    return this->title;
}
void Window::make_current()
{
    Window::current = this;
}
Window* Window::current = nullptr;