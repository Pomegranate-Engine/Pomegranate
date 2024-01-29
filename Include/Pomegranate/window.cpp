#include "window.h"

const Window* Window::current = nullptr;

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

int Window::get_width() const
{
    return this->width;
}

int Window::get_height() const
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
    this->window = SDL_CreateWindow(this->title, this->width, this->height, 0);

    if (!this->window)
    {
        std::cout << "Failed to create window\n" << std::endl;
        return -1;
    }

    this->renderer = SDL_CreateRenderer(this->window, nullptr, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

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

void Window::set_title(const char *title)
{
    this->title = title;
    SDL_SetWindowTitle(this->window, title);
}

void Window::set_size(int width, int height)
{
    this->width = width;
    this->height = height;
    SDL_SetWindowSize(this->window, width, height);
}

void Window::set_icon(const char *path)
{
    SDL_Surface* icon = IMG_Load(path);
    SDL_SetWindowIcon(this->window, icon);
    SDL_DestroySurface(icon);
}
