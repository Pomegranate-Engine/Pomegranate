#ifndef POMEGRANATE_ENGINE_ECS_H
#define POMEGRANATE_ENGINE_ECS_H
#include <typeinfo>
#include <iostream>
#include <vector>
#include <omp.h>
#include<string>
#include "engine.h"
#include <map>
#include <algorithm>
#include <functional>


class Entity;
class Component;
class System;
class EntityGroup;

class Component
{
private:

public:
    virtual ~Component()=default;
    virtual void init(Entity*);
    std::vector<std::pair<const char*,std::pair<const type_info*, void*>>> component_data;
    static std::map<const char*, std::function<Component*()>> component_types;
    template<typename T> static void register_component();
    template<typename T> void push_data(const char* name, void* data);
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
    virtual void pre_tick();
    virtual void tick(Entity*);
    virtual void post_tick();
    virtual void pre_draw();
    virtual void draw(Entity*);
    virtual void post_draw();
    static void add_global_system(System*);
    static void remove_global_system(System*);
    static void global_system_tick();
    static void global_system_draw(std::function<bool(Entity*, Entity*)> sortingFunction);
    static std::map<const char*, std::function<System*()>> system_types;
    template<typename T> static void register_system();
};

class Entity
{
private:
    /* data */
    std::map<const std::type_info*,Component*> components;
    std::vector<EntityGroup*> parents;
public:
    uint64_t id;
    template <typename T> void add_component();
    void remove_component(Component*);
    void add_to_group(EntityGroup*);
    void remove_from_group(EntityGroup*);
    std::vector<EntityGroup*> get_parent_groups();
    template <typename T> T* get_component();
    Component* get_component(const char*);
    template <typename T> T* require_component();
    template <typename T> bool has_component();
    bool has_component(const char*);
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
    void add_group(const EntityGroup&);
    void remove_group(const EntityGroup&);
    void tick();
    void draw(const std::function<bool(Entity*, Entity*)>& sortingFunction);
};

#include "ecs.inl"

#endif //POMEGRANATE_ENGINE_ECS_H
