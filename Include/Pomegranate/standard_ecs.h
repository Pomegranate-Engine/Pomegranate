#ifndef POMEGRANATE_ENGINE_STANDARD_ECS_H
#define POMEGRANATE_ENGINE_STANDARD_ECS_H
#include "ecs.h"
#include "vec_math.h"
#include <SDL_ttf.h>
#include "engine.h"
#include "window.h"
#include "color.h"
#include <map>

class Transform : public Component
{
public:
    Vec2 pos;
    Vec2 scale;
    float rot;
    int z_index;
    Transform();
    static bool draw_sort(Entity* a, Entity* b);
};

class PositionLink : public Component
{
public:
    Transform* link;
    Vec2 offset;
    PositionLink();
};

class RotationLink : public Component
{
public:
    Transform* link;
    float offset;
    RotationLink();
};

class ScaleLink : public Component
{
public:
    Transform* link;
    Vec2 offset;
    ScaleLink();
};

class TransformLinkages : public System
{
public:
    TransformLinkages();
    void tick(Entity* entity) override;
};

class Tag : public Component
{
    std::string tag;
public:
    Tag();
    ~Tag();
    static Entity* get_entity(const std::string& tag);
    static std::vector<Entity*> get_entities(const std::string& tag);
    void set_tag(const std::string& tag);
    static std::unordered_map<std::string, std::vector<Entity*>> tags;
};

#endif //POMEGRANATE_ENGINE_STANDARD_ECS_H
