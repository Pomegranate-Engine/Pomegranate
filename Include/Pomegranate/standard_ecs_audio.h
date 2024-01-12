#ifndef POMEGRANATE_ENGINE_STANDARD_ECS_AUDIO_H
#define POMEGRANATE_ENGINE_STANDARD_ECS_AUDIO_H

#include <SDL.h>
#include<SDL_mixer.h>
#include <iostream>
#include <string>
#include <memory>
#include "engine.h"

class Sample {
public:
    Sample(const char* filePath);
    ~Sample();

    void play();

private:
    Mix_Chunk* soundEffect;
};

#endif //POMEGRANATE_ENGINE_STANDARD_ECS_AUDIO_H
