#include "resource_manager.h"

std::map<std::string, void*> ResourceManager::resources = std::map<std::string, void*>();

Texture::Texture(const char *path)
{
    SDL_Surface* s = IMG_Load(path);
    SDL_Texture* t = SDL_CreateTextureFromSurface(Window::current->get_sdl_renderer(),s);
    texture = t;
    this->width = 0;
    this->height = 0;
    SDL_QueryTexture(texture, nullptr, nullptr,&width,&height);
    SDL_DestroySurface(s);
}
void Texture::load(const char *path)
{
    SDL_Surface* s = IMG_Load(path);
    SDL_Texture* t = SDL_CreateTextureFromSurface(Window::current->get_sdl_renderer(),s);
    texture = t;
    this->width = 0;
    this->height = 0;
    SDL_QueryTexture(texture, nullptr, nullptr,&width,&height);
    SDL_DestroySurface(s);
}
Texture::Texture(int width, int height)
{
    this->width = width;
    this->height = height;
    this->texture = SDL_CreateTexture(Window::current->get_sdl_renderer(),SDL_PixelFormatEnum::SDL_PIXELFORMAT_RGBA32,SDL_TextureAccess::SDL_TEXTUREACCESS_STATIC,width,height);
}

Texture::Texture(SDL_Texture *tex)
{
    texture = tex;
    this->width = 0;
    this->height = 0;
    SDL_QueryTexture(texture, nullptr, nullptr,&width,&height);
}

Texture::~Texture()
{
    SDL_DestroyTexture(texture);
}

Vec2i Texture::get_size() const
{
    return {width,height};
}

void Texture::resize(int nwidth, int nheight)
{
    //TODO: implement when i have internet
}

SDL_Texture *Texture::get_sdl_texture()
{
    return texture;
}

Audio::Audio(Mix_Chunk *chunk)
{
    this->audio = chunk;
}
Audio::Audio(const char *path)
{
    this->audio = Mix_LoadWAV(path);
}
void Audio::load(const char *path)
{
    this->audio = Mix_LoadWAV(path);
}
Audio::~Audio()
{
    Mix_FreeChunk(audio);
}

Mix_Chunk* Audio::get_mix_chunk()
{
    return audio;
}

TTFFont::TTFFont(TTF_Font *ttf)
{
    this->font = ttf;
}
TTFFont::TTFFont(const char *path)
{
    this->font = TTF_OpenFont(path,64);
}
void TTFFont::load(const char *path)
{
    this->font = TTF_OpenFont(path,64);
}
TTFFont::~TTFFont()
{
    TTF_CloseFont(this->font);
}

TTF_Font *TTFFont::get_ttf_font()
{
    return this->font;
}