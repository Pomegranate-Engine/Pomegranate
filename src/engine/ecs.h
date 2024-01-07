#ifndef POMEGRANATE_ENGINE_ECS_H
#define POMEGRANATE_ENGINE_ECS_H
#include <typeinfo>
#include <iostream>
#include <vector>
#include <omp.h>
#include<string>
#include "engine.h"
#include <map>


class Entity;
class Component;
class System;
class EntityGroup;

class Component
{
private:
    /* data */
public:
    virtual ~Component()=default;
};

class System
{
private:
    /* data */
    static std::vector<System*> global_systems;
public:
    System();
    ~System();
    virtual void init(Entity*);
    virtual void tick(Entity*);
    virtual void draw(Entity*);
    static void add_global_system(System*);
    static void remove_global_system(System*);
    static void global_system_tick();
    static void global_system_draw();
};
class Entity
{
private:
    /* data */
    std::map<const std::type_info*,Component*> components;
    std::vector<EntityGroup*> parents;
public:
    uint64_t id;
    void add_component(Component*);
    void remove_component(Component*);
    void add_to_group(EntityGroup*);
    void remove_from_group(EntityGroup*);
    std::vector<EntityGroup*> get_parent_groups();
    template <typename T> T* get_component();
    template <typename T> T* require_component();
    template <typename T> bool has_component();
    Entity();
    ~Entity();
    uint64_t get_id() const;
    //Add a static list of all entities
    static std::vector<Entity*> entities;
    static uint64_t entity_count;
};
class EntityGroup
{
private:
    /* data */
    std::vector<Entity*> entities;
    std::vector<System*> systems;
    std::vector<EntityGroup> child_groups;
public:
    EntityGroup();
    ~EntityGroup();
    void add_entity(Entity*);
    void remove_entity(Entity*);
    void add_system(System*);
    void remove_system(System*);
    void add_group(EntityGroup);
    void remove_group(EntityGroup);
    void tick();
    void draw();
};

#include "ecs.inl"

#endif //POMEGRANATE_ENGINE_ECS_H
