#ifndef POMEGRANATE_ENGINE_RESOURCE_MANAGER_H
#define POMEGRANATE_ENGINE_RESOURCE_MANAGER_H
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <map>
#include "vec_math.h"
#include "window.h"

struct Resource
{
    virtual void load(const char* t){};
};

struct Texture : Resource
{
private:
    SDL_Texture* texture;
    int width;
    int height;
public:
    explicit Texture(const char* path);
    void load(const char* path) override;
    Texture(int width, int height);
    explicit Texture(SDL_Texture* tex);
    ~Texture();
    [[nodiscard]] Vec2i get_size() const;
    void resize(int nwidth, int nheight);
    SDL_Texture* get_sdl_texture();
};

struct Audio : Resource
{
private:
    Mix_Chunk* audio;
public:
    explicit Audio(const char* path);
    void load(const char* path) override;
    explicit Audio(Mix_Chunk* chunk);
    ~Audio();
    Mix_Chunk* get_mix_chunk();
};

struct TTFFont : Resource
{
private:
    TTF_Font* font;
public:
    TTFFont(const char* path);
    void load(const char* path) override;
    TTFFont(TTF_Font* ttf);
    ~TTFFont();
    TTF_Font* get_ttf_font();
};

class ResourceManager
{
private:
    static std::map<std::string,void*> resources;
public:
    template<typename T> static T* load(std::string path);
    template<typename T> static void unload(std::string path); //Unsafe
};

#include "resource_manager.inl"
#endif //POMEGRANATE_ENGINE_RESOURCE_MANAGER_H
