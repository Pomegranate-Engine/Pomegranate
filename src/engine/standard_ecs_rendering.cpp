#include "standard_ecs_rendering.h"

Sprite::Sprite()
{
    this->texture = IMG_LoadTexture(Window::current->get_sdl_renderer(), "res/none.png");
    this->color = Color(255, 255, 255, 255);
}

void Sprite::load_texture(const char *path) {
    this->texture = IMG_LoadTexture(Window::current->get_sdl_renderer(), path);
}

AnimatedSprite::AnimatedSprite()
{
    this->texture = IMG_LoadTexture(Window::current->get_sdl_renderer(), "res/none.png");
    this->frame = 0;
    this->horizontal_frames = 1;
    this->vertical_frames = 1;
    this->x_offset = 0;
    this->y_offset = 0;
    this->color = Color(255, 255, 255, 255);
}

void AnimatedSprite::load_texture(const char *path)
{
    this->texture = IMG_LoadTexture(Window::current->get_sdl_renderer(), path);
}

Camera::Camera()=default;

void Camera::make_current(Entity*entity)
{
    current = entity;
}
Entity* Camera::current = nullptr;

DebugCircle::DebugCircle()
{
    this->radius = 16.0;
    this->color = Color(255, 255, 255, 255);
}

void Render::draw(Entity* entity)
{
    if(entity->has_component<Sprite>())
    {
        Render::sprite(entity);
    }
    if(entity->has_component<DebugCircle>())
    {
        Render::debug_circle(entity);
    }
    if(entity->has_component<AnimatedSprite>())
    {
        Render::animated_sprite(entity);
    }
    if(entity->has_component<Tilemap>())
    {
        Render::tilemap(entity);
    }
}

void Render::sprite(Entity*e) {
    auto t = e->get_component<Transform>();
    auto s = e->get_component<Sprite>();
    SDL_SetRenderDrawColor(Window::current->get_sdl_renderer(), s->color.r,s->color.g,s->color.b, s->color.a);
    SDL_FRect r;
    int w = 0;
    int h = 0;
    SDL_QueryTexture(s->texture, nullptr, nullptr, &w, &h);
    r.w = (float)w*t->scale.x;
    r.h = (float)h*t->scale.y;
    r.x = t->pos.x-r.w/2-Camera::current->get_component<Transform>()->pos.x;
    r.y = t->pos.y-r.h/2-Camera::current->get_component<Transform>()->pos.y;
    auto* center = new SDL_FPoint();
    center->x = r.w/2;
    center->y = r.h/2;
    SDL_RenderTextureRotated(Window::current->get_sdl_renderer(), s->texture, nullptr, &r, t->rot*180.0/3.14159, center, SDL_FLIP_NONE);
}

void Render::animated_sprite(Entity*e) {
    auto t = e->get_component<Transform>();
    auto s = e->get_component<AnimatedSprite>();
    SDL_SetRenderDrawColor(Window::current->get_sdl_renderer(), s->color.r,s->color.g,s->color.b, s->color.a);
    SDL_FRect r = {};
    int w = 0;
    int h = 0;
    SDL_QueryTexture(s->texture, nullptr, nullptr, &w, &h);
    SDL_FRect src = {r.w/s->horizontal_frames*s->frame+s->x_offset, r.h/s->vertical_frames*s->frame+s->y_offset, r.w/s->horizontal_frames, r.h/s->vertical_frames};
    r.w = (float)w*t->scale.x;
    r.h = (float)h*t->scale.y;
    r.x = t->pos.x-r.w/2-Camera::current->get_component<Transform>()->pos.x;
    r.y = t->pos.y-r.h/2-Camera::current->get_component<Transform>()->pos.y;
    auto* center = new SDL_FPoint();
    center->x = r.w/2;
    center->y = r.h/2;
    SDL_RenderTextureRotated(Window::current->get_sdl_renderer(), s->texture, &src, &r, t->rot*180.0/3.14159, center, SDL_FLIP_NONE);
}

void Render::debug_circle(Entity* entity)
{
    auto* t = entity->get_component<Transform>();
    auto* dc = entity->get_component<DebugCircle>();

    SDL_SetRenderDrawColor(Window::current->get_sdl_renderer(), dc->color.r,dc->color.g,dc->color.b, dc->color.a);
    // Calculate angle step for each segment
    double angleStep = 2.0 * 3.14159 / 32;

    // Draw the circle outline
    for (int i = 0; i < 32; ++i) {
        double angle1 = i * angleStep;
        double angle2 = (i + 1) * angleStep;

        // Calculate the coordinates of the two points on the circle
        float x1 = t->pos.x-Camera::current->get_component<Transform>()->pos.x + (dc->radius * cos(angle1));
        float y1 = t->pos.y-Camera::current->get_component<Transform>()->pos.y + (dc->radius * sin(angle1));
        float x2 = t->pos.x-Camera::current->get_component<Transform>()->pos.x + (dc->radius * cos(angle2));
        float y2 = t->pos.y-Camera::current->get_component<Transform>()->pos.y + (dc->radius * sin(angle2));

        // Draw a line between the two points to create the circle outline
        SDL_RenderLine(Window::current->get_sdl_renderer(), x1, y1, x2, y2);
    }
}

void Render::tilemap(Entity* entity)
{
    auto* t = entity->get_component<Transform>();
    auto* map = entity->get_component<Tilemap>();

    auto* camera_transform = Camera::current->get_component<Transform>();

    SDL_SetRenderDrawColor(Window::current->get_sdl_renderer(), map->color.r,map->color.g,map->color.b, map->color.a);
    // Calculate angle step for each segment
    for (int z = 0; z < map->get_layer_count(); ++z)
    {
        int i = 0;
        for (int y = 0; y < map->height; ++y) {
            for (int x = 0; x < map->width; ++x) {
                if (map->get_tile(i, z).x != -1) {
                    vec2i tile = map->get_tile(i,z);
                    SDL_FRect src = {(float) tile.x * (float) map->tileset_tile_width,
                                     (float) tile.y * (float) map->tileset_tile_height, (float) map->tileset_tile_width,
                                     (float) map->tileset_tile_height};
                    SDL_FRect dst = {(float) (x * map->tileset_tile_width), (float) (y * map->tileset_tile_height),
                                     (float) map->tileset_tile_width, (float) map->tileset_tile_height};
                    dst.x *= t->scale.x;
                    dst.y *= t->scale.y;
                    dst.w *= t->scale.x;
                    dst.h *= t->scale.y;
                    dst.x -= camera_transform->pos.x;
                    dst.y -= camera_transform->pos.y;
                    dst.x += t->pos.x;
                    dst.y += t->pos.y;

                    SDL_RenderTexture(Window::current->get_sdl_renderer(), map->tileset_texture, &src, &dst);
                }
                i++;
            }
            i++;
        }
    }
}

Tilemap::Tilemap()
{
    this->width = 256;
    this->height = 256;
    this->tileset_horizontal_tiles=0;
    this->tileset_vertical_tiles=0;
    this->tileset_tile_width=0;
    this->tileset_tile_height=0;
    this->color = Color(255, 255, 255, 255);
    this->tiles = std::vector<vec2i*>();
    tiles.push_back(new vec2i[width*height]);
    for(int i = 0; i < width*height; i++)
    {
        tiles[0][i] = vec2i(-1,-1);
    }
    this->tileset_texture = nullptr;
}

void Tilemap::load_texture(const char *path)
{
    this->tileset_texture = IMG_LoadTexture(Window::current->get_sdl_renderer(), path);
}

void Tilemap::set_tile(vec2i pos, vec2i tile, int layer)
{
    int p = pos.x+pos.y*(width+1);
    if(p<width*height)
    {
        tiles[layer][p] = tile;
    }
    else
    {
        print_error("Tilemap index out of bounds");
    }
}

vec2i Tilemap::get_tile(vec2i pos, int layer)
{
    int p = pos.x+pos.y*(width+1);
    if(p<width*height)
    {
        return tiles[layer][p];
    }
    else
    {
        return {-1,-1};
    }
}

void Tilemap::set_tileset_tile_size(int w, int h)
{
    this->tileset_tile_width = 16;
    this->tileset_tile_height = 16;
    int wid = 0;
    int hei = 0;
    SDL_QueryTexture(this->tileset_texture, nullptr, nullptr, &wid, &hei);
    this->tileset_horizontal_tiles = 17;
    this->tileset_vertical_tiles = 8;
}

vec2i Tilemap::get_tile(int index, int layer)
{
    return (tiles[layer])[index];
}

void Tilemap::fill_tile(vec2i a, vec2i b, vec2i tile, int layer)
{
    for (int y = a.y; y <= b.y; ++y)
    {
        for (int x = a.x; x <= b.x; ++x)
        {
            set_tile(vec2i(x,y), tile, layer);
        }
    }
}

void Tilemap::place_multitile(vec2i pos, vec2i tile_a, vec2i tile_b, int layer)
{
    for (int y = tile_a.y; y <= tile_b.y; ++y)
    {
        for (int x = tile_a.x; x <= tile_b.x; ++x)
        {
            set_tile(vec2i(x-tile_a.x+pos.x,y-tile_a.y+pos.y), vec2i(x,y),layer);
        }
    }
}

void Tilemap::add_layer()
{
    tiles.push_back(new vec2i[width*height]);
    for(int i = 0; i < width*height; i++)
    {
        tiles[tiles.size()-1][i] = vec2i(-1,-1);
    }
}

int Tilemap::get_layer_count()
{
    return this->tiles.size();
}
