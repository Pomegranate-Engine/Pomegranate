#include "standard_ecs.h"

Transform::Transform()
{
    this->pos = Vec2(0.0, 0.0);
    this->scale = Vec2(1.0, 1.0);
    this->rot = 0.0;
    this->z_index = 0;

}

bool Transform::draw_sort(Entity* a, Entity* b)
{
    if(a->has_component<Transform>() && b->has_component<Transform>())
    {
        return a->get_component<Transform>()->z_index < b->get_component<Transform>()->z_index;
    }
    else
    {
        return false;
    }
}

PositionLink::PositionLink()
{
    this->link = nullptr;
    this->offset = Vec2(0.0, 0.0);
}

RotationLink::RotationLink()
{
    this->link = nullptr;
    this->offset = 0.0;
}

ScaleLink::ScaleLink()
{
    this->link = nullptr;
    this->offset = Vec2(0.0, 0.0);
}

void TransformLinkages::tick(Entity *entity)
{
    if(entity->has_component<Transform>())
    {
        if (entity->has_component<PositionLink>())
        {
            auto *t = entity->get_component<Transform>();
            auto *l = entity->get_component<PositionLink>();
            t->pos = l->link->pos + l->offset;
        }
        if (entity->has_component<RotationLink>())
        {
            auto *t = entity->get_component<Transform>();
            auto *l = entity->get_component<RotationLink>();
            t->rot = l->link->rot + l->offset;
        }
        if (entity->has_component<ScaleLink>())
        {
            auto *t = entity->get_component<Transform>();
            auto *l = entity->get_component<ScaleLink>();
            t->scale = l->link->scale + l->offset;
        }
    }
}
