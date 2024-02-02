#include "standard_ecs.h"

//Globals
std::unordered_map<std::string, std::vector<Entity*>> Tag::tags = std::unordered_map<std::string, std::vector<Entity*>>();

Transform::Transform()
{
    this->pos = Vec2(0.0, 0.0);
    this->scale = Vec2(1.0, 1.0);
    this->rot = 0.0;
    this->z_index = 0;
    push_data<Vec2>("pos", &this->pos);
    push_data<Vec2>("scale", &this->scale);
    push_data<float>("rot",  &this->rot);
    push_data<int>("z_index",  &this->z_index);
    register_component<Transform>();
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
    register_component<PositionLink>();
    //TODO: Add support for entity links in lua
    push_data<Vec2>("offset", &this->offset);
}

RotationLink::RotationLink()
{
    this->link = nullptr;
    this->offset = 0.0;
    register_component<RotationLink>();
    //TODO: Add support for entity links in lua
    push_data<float>("offset", &this->offset);
}

ScaleLink::ScaleLink()
{
    this->link = nullptr;
    this->offset = Vec2(0.0, 0.0);
    register_component<ScaleLink>();
    //TODO: Add support for entity links in lua
    push_data<Vec2>("offset", &this->offset);
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

TransformLinkages::TransformLinkages()
{
    register_system<TransformLinkages>();
}

Tag::Tag()
{

}

Tag::~Tag()
{

}

Entity *Tag::get_entity(const std::string &tag)
{
    return tags[tag].front();
}

std::vector<Entity *> Tag::get_entities(const std::string &tag)
{
    return tags[tag];
}

void Tag::set_tag(const std::string &t)
{
    this->tag = t;
    if(tags[t].empty())
    {
        tags[t] = std::vector<Entity*>();
    }
    if(std::find(tags[t].begin(), tags[t].end(), reinterpret_cast<Entity *const>(this)) == tags[t].end())
    {
        tags[t].push_back(reinterpret_cast<Entity *const>(this));
    }
    //Remove it from other tags
    for(auto & tag : tags)
    {
        if(tag.first == t)
        {
            continue;
        }
        if(std::find(tag.second.begin(), tag.second.end(), reinterpret_cast<Entity *const>(this)) != tag.second.end())
        {
            tag.second.erase(std::remove(tag.second.begin(), tag.second.end(), reinterpret_cast<Entity *const>(this)), tag.second.end());
        }
    }
}
