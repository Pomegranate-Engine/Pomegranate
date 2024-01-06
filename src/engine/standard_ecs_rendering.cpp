#include "standard_ecs_rendering.h"

Sprite::Sprite()
{
    this->texture = IMG_LoadTexture(Window::current->get_sdl_renderer(), "res/none.png");
    this->color = new Color(255, 255, 255, 255);
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
    this->color = new Color(255, 255, 255, 255);
}

void AnimatedSprite::load_texture(const char *path)
{
    this->texture = IMG_LoadTexture(Window::current->get_sdl_renderer(), path);
}

Text::Text()
{
    this->font = TTF_OpenFont("res/sans.ttf", 16);
    this->text = "text";
    this->color = new Color(255, 255, 255, 255);
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
    this->color = new Color(255, 255, 255, 255);
}

void Render::draw(Entity* entity)
{
    if(entity->has_component<Sprite>())
    {
        Render::sprite(entity);
    }
    if(entity->has_component<Text>())
    {
        Render::text(entity);
    }
    if(entity->has_component<DebugCircle>())
    {
        Render::debug_circle(entity);
    }
    if(entity->has_component<AnimatedSprite>())
    {
        Render::animated_sprite(entity);
    }
}

void Render::text(Entity*e) {
    auto text = e->get_component<Text>();
    auto trans = e->get_component<Transform>();

    SDL_Color textColor = {(Uint8)text->color->r,(Uint8)text->color->g,(Uint8)text->color->b,(Uint8)text->color->a}; // White color

    // Create an SDL surface from the text
    SDL_Surface* surface = TTF_RenderText_Blended(text->font, text->text.c_str(), textColor);
    if (!surface) {
        // Handle error
        return;
    }

    // Create an SDL texture from the surface
    SDL_Texture* tex = SDL_CreateTextureFromSurface(Window::current->get_sdl_renderer(), surface);
    if (!tex) {
        // Handle error
        SDL_DestroySurface(surface);
        return;
    }

    // Set the position and dimensions of the text
    SDL_FRect destinationRect = {(trans->pos.x-Camera::current->get_component<Transform>()->pos.x), (trans->pos.y-Camera::current->get_component<Transform>()->pos.y), (float)surface->w, (float)surface->h};

    // Render the text to the screen
    SDL_RenderTexture(Window::current->get_sdl_renderer(),tex, nullptr,&destinationRect);

    // Clean up
    SDL_DestroyTexture(tex);
    SDL_DestroySurface(surface);
}

void Render::sprite(Entity*e) {
    auto t = e->get_component<Transform>();
    auto s = e->get_component<Sprite>();
    SDL_SetRenderDrawColor(Window::current->get_sdl_renderer(), s->color->r,s->color->g,s->color->b, s->color->a);
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
    SDL_SetRenderDrawColor(Window::current->get_sdl_renderer(), s->color->r,s->color->g,s->color->b, s->color->a);
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

    SDL_SetRenderDrawColor(Window::current->get_sdl_renderer(), dc->color->r,dc->color->g,dc->color->b, dc->color->a);
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