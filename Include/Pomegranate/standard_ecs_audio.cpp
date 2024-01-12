#include "standard_ecs_audio.h"

Sample::Sample(const char* filePath)
{
    soundEffect = Mix_LoadWAV(filePath);
    if (!soundEffect)
    {
        print_error(std::string("Failed to play sound effect: ") + Mix_GetError());
    }
}

Sample::~Sample()
{
    Mix_FreeChunk(soundEffect);
}

void Sample::play()
{
    if (Mix_PlayChannel(-1, soundEffect, 0) == -1)
    {
        print_error(std::string("Failed to play sound effect: ") + Mix_GetError());
    }
}