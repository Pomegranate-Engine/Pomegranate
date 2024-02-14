#include "window.h"

#include "resource_manager.h"

namespace Pomegranate
{
    Window* Window::current = nullptr;

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
    SDL_Texture* Window::get_render_texture() const
    {
        return this->render_texture;
    }

    int Window::open()
    {
        bool HIGH_DPI = false;
#if defined(__APPLE__) //TODO: Implement better solution for HIGH_DPI
        HIGH_DPI = true;
#endif
        int flags = SDL_WINDOW_RESIZABLE;
        if(HIGH_DPI)
        {
            flags |= SDL_WINDOW_HIGH_PIXEL_DENSITY;
        }
        this->window = SDL_CreateWindow(this->title, this->width, this->height, flags); //TODO: Add HIGH_DPI support
        if (!this->window)
        {
            std::cout << "Failed to create window\n" << std::endl;
            return -1;
        }

        this->renderer = SDL_CreateRenderer(this->window, nullptr, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        //Set render texture
        this->render_texture = SDL_CreateTexture(this->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, this->width, this->height);

        if(HIGH_DPI)
        {
            SDL_SetRenderScale(renderer, 2, 2);
        }
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

    void Window::set_title(const char *new_title)
    {
        this->title = new_title;
        SDL_SetWindowTitle(this->window, new_title);
    }

    void Window::set_size(int new_width, int new_height)
    {
        this->width = new_width;
        this->height = new_height;
        SDL_SetWindowSize(this->window, width, height);
        //Update the renderer
        SDL_DestroyTexture(this->render_texture);
        this->render_texture = SDL_CreateTexture(this->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, this->width, this->height);
    }
    void Window::display()
    {
        SDL_SetRenderTarget(Window::current->get_sdl_renderer(), nullptr);
        SDL_RenderTexture(Window::current->get_sdl_renderer(), Window::current->get_render_texture(), nullptr, nullptr);
        SDL_RenderPresent(Window::current->get_sdl_renderer()); //Present
    }
    void Window::set_icon(const char *path)
    {
        SDL_Surface* icon = IMG_Load(path);
        SDL_SetWindowIcon(this->window, icon);
        SDL_DestroySurface(icon);
    }
    void Window::process_event(SDL_Event* event)
    {
        if(event->type == SDL_EVENT_WINDOW_RESIZED)
        {
            print_info("Window resized");
            Window::current->set_size(event->window.data1, event->window.data2);
        }
    }
}
