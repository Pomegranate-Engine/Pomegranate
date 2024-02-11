#ifndef POMEGRANATE_ENGINE_RESOURCE_MANAGER_H
#define POMEGRANATE_ENGINE_RESOURCE_MANAGER_H
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <map>
#include "vec_math.h"
#include "window.h"
namespace Pomegranate
{
    struct Resource
    {
        std::string path;
    };

    class ResourceManager
    {
    private:
        static std::map<std::string,void*> resources;
    public:
        template<typename T> static T* load(std::string path);
        template<typename T> static void unload(std::string path); //Unsafe
    };
}

#include "resource_manager.inl"
#endif //POMEGRANATE_ENGINE_RESOURCE_MANAGER_H
