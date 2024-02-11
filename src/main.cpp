#include <SDL.h>
#include <SDL_image.h>
#include <Pomegranate/pomegranate.h>
#include<Pomegranate/lua_wrapper.h>
#include<imgui.h>
#include<backends/imgui_impl_sdl3.h>
#include<backends/imgui_impl_sdlrenderer3.h>
using namespace Pomegranate;
//Main window
Window main_window = Window("Window", 1024, 720);
bool use_lua_camera_controller = false;
Entity* Info_Text;
Entity* checkbox;

//Example program
#include "entities.cpp"
#include "systems.cpp"

//Game example
#include "init.cpp"
#include "game.cpp"

int main(int argc, char* argv[])
{
    init();
    auto* scene = build_scene();

    float tick_time = 0.0;
    bool is_running = true;
    SDL_Event event;

    while (is_running)
    {
        Uint64 start = SDL_GetPerformanceCounter(); //For delta time

        //- - - - - # SDL2 Stuff # - - - - -

        //Poll events
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL3_ProcessEvent(&event);
            InputManager::process_event(event);
            if (event.type == SDL_EVENT_QUIT) {
                is_running = false;
            }
        }

        //- - - - - # UPDATE # - - - - -

        if (tick_time > 0.016)
        {
            tick_time = 0.0;

            tick(scene);
        }

        //- - - - - # RENDERING # - - - - -

        //Clear
        if(checkbox->get_component<UICheckbox>()->checked)
        {
            //Set imgui theme
            ImGui::StyleColorsLight();
            SDL_SetRenderDrawColor(Window::current->get_sdl_renderer(), 222, 222, 222, SDL_ALPHA_OPAQUE);
        }
        else
        {
            //Set imgui theme
            ImGui::StyleColorsDark();
            SDL_SetRenderDrawColor(Window::current->get_sdl_renderer(), 7, 14, 14, SDL_ALPHA_OPAQUE);
        }
        SDL_RenderClear(Window::current->get_sdl_renderer());
        SDL_SetRenderDrawColor(Window::current->get_sdl_renderer(), 255, 255, 255, 255);

        //Begin imgui
        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        draw(scene);


        //Draw imgui
        ImGui::Render();
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData());

        SDL_RenderPresent(Window::current->get_sdl_renderer()); //Present

        //- - - - - # AFTER FRAME # - - - - -

        //Calculate delta time
        Uint64 end = SDL_GetPerformanceCounter();
        float secondsElapsed = (float)(end - start) / (float)SDL_GetPerformanceFrequency();
        delta_time = secondsElapsed;
        tick_time += delta_time;
    }
    main_window.close();
    pomegranate_quit(); //Cleanup
    return 0;
}
