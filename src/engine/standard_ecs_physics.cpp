#include "standard_ecs_physics.h"

PhysicsObject::PhysicsObject()
{
    this->linear_velocity = Vec2(0.0, 0.0);
    this->angular_velocity = 0.0;
    this->mass = 1.0;
    this->gravity_scale = 1.0;
    this->continuous_collision_detection = false;
    this->use_collision = true;
    this->drag = 0.0;
    this->body_type = PHYSICS_BODY_TYPE_RIGID;
}

Vec2 PhysicsObject::gravity = Vec2(0.0, 980.0);

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
            float x = 0;
            float y = 0;
            SDL_GetMouseState(&x, &y);


            auto *p = entity->get_component<PhysicsObject>();

            Vec2 acceleration = Vec2(0.0, 0.0);
            acceleration += PhysicsObject::gravity*p->gravity_scale;
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
int RigidBody::substeps = 12;

CollisionShape::CollisionShape()
{
    this->shape_type = COLLISION_SHAPE_TYPE_CIRCLE;
    this->radius = 16.0;
    this->restitution = 0.0;
    this->size = Vec2(32.0, 32.0);
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