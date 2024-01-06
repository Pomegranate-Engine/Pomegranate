#ifndef POMEGRANATE_ENGINE_STANDARD_ECS_RENDERING_H
#define POMEGRANATE_ENGINE_STANDARD_ECS_RENDERING_H

#include "ecs.h"
#include "vec_math.h"
#include "color.h"
#include "window.h"
#include "standard_ecs.h"

class DebugCircle : public Component
{
public:
    float radius;
    Color* color;
    DebugCircle();
};

class Sprite : public Component
{
public:
    SDL_Texture* texture;
    Color* color;
    Sprite();
    void load_texture(const char* path);
};

class AnimatedSprite : public Component
{
public:
    SDL_Texture* texture;
    Color* color;
    int frame;
    int horizontal_frames;
    int vertical_frames;
    int x_offset;
    int y_offset;
    AnimatedSprite();
    void load_texture(const char* path);
};

class Text : public Component
{
public:
    TTF_Font* font;
    std::string text;
    Color* color;
    Text();
};

class Render: public System
{
public:
    void draw(Entity* entity) override;
    static void sprite(Entity*);
    static void text(Entity*);
    static void debug_circle(Entity*);
    void animated_sprite(Entity *e);
};

class Camera: public Component
{
public:
    Camera();
    static void make_current(Entity*entity);
    static Entity* current;
};


#endif //POMEGRANATE_ENGINE_STANDARD_ECS_RENDERING_H
