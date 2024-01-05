#ifndef POMEGRANATE_ENGINE_STANDARD_ECS_H
#define POMEGRANATE_ENGINE_STANDARD_ECS_H
#include "ecs.h"
#include "vec_math.h"
#include <SDL_ttf.h>
#include "engine.h"
#include "window.h"

class Transform : public Component
{
public:
    vec2 pos;
    vec2 scale;
    float rot;
    Transform();
};

enum PhysicsBodyType
{
    PHYSICS_BODY_TYPE_KINEMATIC,
    PHYSICS_BODY_TYPE_RIGID,
    PHYSICS_BODY_TYPE_STATIC
};
enum CollisionShapeType
{
    COLLISION_SHAPE_TYPE_RECTANGLE,
    COLLISION_SHAPE_TYPE_CIRCLE
};

class PhysicsObject : public Component
{
public:
    vec2 linear_velocity;
    float angular_velocity;
    float mass;
    float gravity_scale;
    float drag;
    bool continuous_collision_detection;
    bool use_collision;
    PhysicsBodyType body_type;
    PhysicsObject();
};

class CollisionShape : public Component
{
public:
    CollisionShapeType shape_type;
    float radius;
    vec2 size;
    float restitution;
    CollisionShape();
};

class DebugCircle : public Component
{
public:
    float radius;
    DebugCircle();
};

class Sprite : public Component
{
public:
    SDL_Texture* texture;
    Sprite();
    void load_texture(const char* path);
};

class Text : public Component
{
public:
    TTF_Font* font;
    std::string text;
    Text();
};

class Move: public System
{
public:
    void tick(Entity* entity) override;
};

class KinematicBody: public System
{
public:
    void tick(Entity* entity) override;
};

class RigidBody: public System
{
public:
    void tick(Entity* entity) override;
    static bool check_collision(Entity* a, Entity* b);
    static void resolve_collision(Entity* a, Entity* b);
    static int substeps;
};

class Render: public System
{
public:
    void draw(Entity* entity) override;
    static void sprite(Entity*);
    static void text(Entity*);
    static void debug_circle(Entity*);
};

class Camera: public Component
{
public:
    Camera();
    static void make_current(Entity*entity);
    static Entity* current;
};
#endif //POMEGRANATE_ENGINE_STANDARD_ECS_H
