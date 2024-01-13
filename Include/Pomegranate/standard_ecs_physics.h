#ifndef POMEGRANATE_ENGINE_STANDARD_ECS_PHYSICS_H
#define POMEGRANATE_ENGINE_STANDARD_ECS_PHYSICS_H

#include "ecs.h"
#include "vec_math.h"
#include "standard_ecs.h"


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
    Vec2 linear_velocity;
    float angular_velocity;
    float mass;
    float gravity_scale;
    float drag;
    bool continuous_collision_detection;
    bool use_collision;
    PhysicsBodyType body_type;
    PhysicsObject();
    static Vec2 gravity;
};

class CollisionShape : public Component
{
public:
    CollisionShapeType shape_type;
    float radius;
    Vec2 size;
    float restitution;
    CollisionShape();
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
    static int sub_steps;
};

#endif //POMEGRANATE_ENGINE_STANDARD_ECS_PHYSICS_H