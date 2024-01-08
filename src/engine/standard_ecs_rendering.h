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
    Color color;
    DebugCircle();
};

class Sprite : public Component
{
public:
    SDL_Texture* texture;
    Color color;
    Sprite();
    void load_texture(const char* path);
};

class AnimatedSprite : public Component
{
public:
    SDL_Texture* texture;
    Color color;
    int frame;
    int horizontal_frames;
    int vertical_frames;
    int x_offset;
    int y_offset;
    AnimatedSprite();
    void load_texture(const char* path);
};

class Render: public System
{
public:
    void draw(Entity* entity) override;
    static void sprite(Entity*);
    static void text(Entity*);
    static void debug_circle(Entity*);
    static void animated_sprite(Entity *e);

    static void tilemap(Entity *entity);
};

class Camera: public Component
{
public:
    Camera();
    static void make_current(Entity*entity);
    static Entity* current;
};

class Tilemap : public Component
{
private:
    std::vector<vec2i*> tiles;
public:
    SDL_Texture* tileset_texture;
    int tileset_horizontal_tiles;
    int tileset_vertical_tiles;
    int tileset_tile_width;
    int tileset_tile_height;
    Color color;
    int width;
    int height;
    Tilemap();
    void load_texture(const char* path);
    void add_layer();
    int get_layer_count();
    void set_tileset_tile_size(int width, int height);
    void set_tile(vec2i pos, vec2i tile, int layer=0);
    void fill_tile(vec2i a, vec2i b, vec2i tile,int layer=0);
    void place_multitile(vec2i pos, vec2i tile_a, vec2i tile_b,int layer=0);
    vec2i get_tile(vec2i pos,int layer=0);
    vec2i get_tile(int index,int layer=0);
};


#endif //POMEGRANATE_ENGINE_STANDARD_ECS_RENDERING_H
