#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL3/SDL_vulkan.h>
#include <Pomegranate/pomegranate.h>
#include<Pomegranate/lua_wrapper.h>
#include<imgui.h>
#include<backends/imgui_impl_sdl3.h>
#include<backends/imgui_impl_sdlrenderer3.h>

#include <vulkan/vulkan.hpp>

//Main window
Window main_window = Window("Window", 1024, 720);
bool use_lua_camera_controller = true;

//Example program
#include "entities.cpp"
#include "systems.cpp"

//Game example
#include "init.cpp"
#include "game.cpp"

int main(int argc, char* argv[])
{
    //Init vulkan
    SDL_Vulkan_LoadLibrary(nullptr);
    init();

    uint32_t extensionCount;
    Uint32 extcount = 0;
    char* extensionNames = (char *) SDL_Vulkan_GetInstanceExtensions(&extcount, &extensionCount, nullptr);
    const VkInstanceCreateInfo instInfo = {
            VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO, // sType
            nullptr,                                // pNext
            0,                                      // flags
            nullptr,                                // pApplicationInfo
            0,                                      // enabledLayerCount
            nullptr,                                // ppEnabledLayerNames
            extensionCount,                         // enabledExtensionCount
            reinterpret_cast<const char *const *>(extensionNames),                         // ppEnabledExtensionNames
    };
    VkInstance vkInst;
    vkCreateInstance(&instInfo, nullptr,&vkInst);


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
        SDL_SetRenderDrawColor(Window::current->get_sdl_renderer(), 7, 14, 14, SDL_ALPHA_OPAQUE);
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

    pomegranate_quit(); //Cleanup
    return 0;
}
