#include "standard_ecs.h"

Transform::Transform()
{
    this->pos = vec2(0.0, 0.0);
    this->scale = vec2(1.0, 1.0);
    this->rot = 0.0;
}


SDL_Texture* texture;
Sprite::Sprite()
{
    this->texture = IMG_LoadTexture(Window::current->get_sdl_renderer(), "res/none.png");
}

void Sprite::load_texture(const char *path) {
    this->texture = IMG_LoadTexture(Window::current->get_sdl_renderer(), path);
}

void Move::tick(Entity* entity)
{
    auto* t = entity->get_component<Transform>();
    t->pos.x = 256.0f + cos(t->rot)*196;
    t->pos.y = 256.0f + sin(t->rot)*196;
    t->scale = vec2(1.0, 1.0) * 2.0;
    t->rot += 0.016*3.14159*0.5;
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
}

void Render::text(Entity*e) {
    auto text = e->get_component<Text>();
    auto trans = e->get_component<Transform>();

    SDL_Color textColor = {255, 255, 255, 255}; // White color

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
        SDL_FreeSurface(surface);
        return;
    }

    // Set the position and dimensions of the text
    SDL_Rect destinationRect = {static_cast<int>(trans->pos.x-Camera::current->get_component<Transform>()->pos.x), static_cast<int>(trans->pos.y-Camera::current->get_component<Transform>()->pos.y), surface->w, surface->h};

    // Render the text to the screen
    SDL_RenderCopy(Window::current->get_sdl_renderer(), tex, nullptr, &destinationRect);

    // Clean up
    SDL_DestroyTexture(tex);
    SDL_FreeSurface(surface);
}

void Render::sprite(Entity*e) {
    auto t = e->get_component<Transform>();
    auto s = e->get_component<Sprite>();
    SDL_SetRenderDrawColor(Window::current->get_sdl_renderer(), 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_Rect r;
    SDL_QueryTexture(s->texture, nullptr, nullptr, &r.w, &r.h);
    r.w *= (int)t->scale.x;
    r.h *= (int)t->scale.y;
    r.x = (int)t->pos.x-r.w/2-(int)Camera::current->get_component<Transform>()->pos.x;
    r.y = (int)t->pos.y-r.h/2-(int)Camera::current->get_component<Transform>()->pos.y;
    auto* center = new SDL_Point();
    center->x = r.w/2;
    center->y = r.h/2;
    SDL_RenderCopyEx(Window::current->get_sdl_renderer(), s->texture, nullptr, &r, t->rot*180.0/3.14159, center, SDL_FLIP_NONE);
}

void Render::debug_circle(Entity* entity)
{
    auto* t = entity->get_component<Transform>();
    auto* dc = entity->get_component<DebugCircle>();
    // Calculate angle step for each segment
    double angleStep = 2.0 * M_PI / 32;

    // Draw the circle outline
    for (int i = 0; i < 32; ++i) {
        double angle1 = i * angleStep;
        double angle2 = (i + 1) * angleStep;

        // Calculate the coordinates of the two points on the circle
        int x1 = (int)t->pos.x-Camera::current->get_component<Transform>()->pos.x + static_cast<int>(dc->radius * cos(angle1));
        int y1 = (int)t->pos.y-Camera::current->get_component<Transform>()->pos.y + static_cast<int>(dc->radius * sin(angle1));
        int x2 = (int)t->pos.x-Camera::current->get_component<Transform>()->pos.x + static_cast<int>(dc->radius * cos(angle2));
        int y2 = (int)t->pos.y-Camera::current->get_component<Transform>()->pos.y + static_cast<int>(dc->radius * sin(angle2));

        // Draw a line between the two points to create the circle outline
        SDL_RenderDrawLine(Window::current->get_sdl_renderer(), x1, y1, x2, y2);
    }
}

PhysicsObject::PhysicsObject()
{
    this->linear_velocity = vec2(0.0, 0.0);
    this->angular_velocity = 0.0;
    this->mass = 1.0;
    this->gravity_scale = 100.0;
    this->continuous_collision_detection = false;
    this->use_collision = true;
    this->drag = 0.0;
    this->body_type = PHYSICS_BODY_TYPE_RIGID;
}

void KinematicBody::tick(Entity *entity)
{

}

void RigidBody::tick(Entity *entity)
{
    if(entity->has_component<PhysicsObject>() && entity->get_component<PhysicsObject>()->body_type == PHYSICS_BODY_TYPE_RIGID)
    {
        for (int i = 0; i < RigidBody::substeps; ++i)
        {
            float delta = 0.016/RigidBody::substeps;
            auto *t = entity->get_component<Transform>();
            int x = 0;
            int y = 0;
            SDL_GetMouseState(&x, &y);


            auto *p = entity->get_component<PhysicsObject>();

            vec2 acceleration = vec2(0.0, 0.0);
            acceleration += vec2(0.0, 1.0) * 9.8 * p->gravity_scale;
            acceleration += p->linear_velocity * -p->drag;
            p->linear_velocity += acceleration * delta;

            t->pos += p->linear_velocity * delta;
            t->rot += p->angular_velocity * delta;
            //print_info("Velocity: " + std::to_string(p->linear_velocity.x) + ", " + std::to_string(p->linear_velocity.y));



            for (auto &entitie: Entity::entities)
            {
                if (entitie->has_component<CollisionShape>())
                {
                    if (entity->get_id() != entitie->get_id())
                    {
                        if (RigidBody::check_collision(entity, entitie)) {
                            RigidBody::resolve_collision(entity, entitie);
                        }
                    }
                }
            }
        }
    }
}
int RigidBody::substeps = 64;

CollisionShape::CollisionShape()
{
    this->shape_type = COLLISION_SHAPE_TYPE_CIRCLE;
    this->radius = 16.0;
    this->restitution = 0.0;
    this->size = vec2(32.0, 32.0);
}

bool RigidBody::check_collision(Entity* a, Entity* b)
{
    auto a_shape = a->get_component<CollisionShape>();
    auto b_shape = b->get_component<CollisionShape>();
    if(a_shape!= nullptr && b_shape != nullptr && a_shape->shape_type == COLLISION_SHAPE_TYPE_CIRCLE && b_shape->shape_type == COLLISION_SHAPE_TYPE_CIRCLE)
    {
        auto a_tran = a->get_component<Transform>();
        float a_size = a_tran->scale.x;
        auto b_tran = b->get_component<Transform>();
        float b_size = b_tran->scale.x;
        return a_tran->pos.distance_to(b_tran->pos) < a_shape->radius*a_size + b_shape->radius*b_size;
    }
    return false;
}
void RigidBody::resolve_collision(Entity* a, Entity* b) {
    auto *a_t = a->get_component<Transform>();
    auto *b_t = b->get_component<Transform>();
    auto *a_c = a->get_component<CollisionShape>();
    auto *b_c = b->get_component<CollisionShape>();
    auto *a_p = a->get_component<PhysicsObject>();
    auto *b_p = b->get_component<PhysicsObject>();
    float a_size = a_t->scale.x;
    float b_size = b_t->scale.x;
    // Calculate the vector between the centers of the two circles
    float dx = b_t->pos.x - a_t->pos.x;
    float dy = b_t->pos.y - a_t->pos.y;
    float distance = a_t->pos.distance_to(b_t->pos);

    // Calculate the unit normal and unit tangent vectors
    float nx = dx / distance;
    float ny = dy / distance;
    float tx = -ny; // Tangent is 90 degrees to the normal
    float ty = nx; // Tangent is 90 degrees to the normal

    // Calculate the relative velocity
    float relativeVelocityX = b_p->linear_velocity.x - a_p->linear_velocity.x;
    float relativeVelocityY = b_p->linear_velocity.y - a_p->linear_velocity.y;
    float relativeVelocityDot = relativeVelocityX * nx + relativeVelocityY * ny;

    // Calculate the impulse
    float impulse = (2.0f * (a_p->mass * b_p->mass * relativeVelocityDot)) /
                    ((a_p->mass + b_p->mass) * distance);

    float overlap = (a_c->radius*a_size + b_c->radius*b_size) - distance;
    // Update velocities based on the impulse
    if (a_p->body_type == PHYSICS_BODY_TYPE_RIGID) {
        a_p->linear_velocity.x += (impulse / a_p->mass) * nx;
        a_p->linear_velocity.y += (impulse / a_p->mass) * ny;
        a_t->pos.x -= overlap * nx;
        a_t->pos.y -= overlap * ny;
    }
    if (b_p->body_type == PHYSICS_BODY_TYPE_RIGID)
    {
        b_p->linear_velocity.x -= (impulse / b_p->mass) * nx;
        b_p->linear_velocity.y -= (impulse / b_p->mass) * ny;
        b_t->pos.x += overlap * nx;
        b_t->pos.y += overlap * ny;
    }

    // Move the circles away from each other by a small distance to eliminate overlap
}

Text::Text()
{
    this->font = TTF_OpenFont("res/sans.ttf", 16);
    this->text = "text";
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
}
