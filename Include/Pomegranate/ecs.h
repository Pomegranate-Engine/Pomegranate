#ifndef POMEGRANATE_ENGINE_ECS_H
#define POMEGRANATE_ENGINE_ECS_H
#include <typeinfo>
#include <iostream>
#include <vector>
#include <omp.h>
#include<string>
#include "engine.h"
#include <unordered_map>
#include <algorithm>
#include <functional>
#include <utility>
#include <typeindex>

extern "C"
{
#include <Lua/lua.h>
#include <Lua/lauxlib.h>
#include <Lua/lualib.h>
}


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
    std::unordered_map<std::string,std::pair<const std::type_info*, void*>> component_data;
    static std::unordered_map<const char*, std::function<Component*()>> component_types;
    template<typename T> static void register_component();
    template<typename T> void push_data(std::string name, void* data);
    template<typename T> T get(std::string name);
    template<typename T> void set(const char* name, T value);
};

//For lua wrapper
class LuaComponent : public Component
{
public:
    uint32_t real_type;
    static std::unordered_map<std::string, int> lua_component_types;
    static LuaComponent* current;
    lua_State* state;
    bool loaded = false;
    LuaComponent();
    void load_script(const char* path);
};


class System
{
private:
    /* data */
    static std::vector<System*> global_systems;
public:
    bool active = true;
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
    static std::unordered_map<const char*, std::function<System*()>> system_types;
    template<typename T> static void register_system();
};

class Entity
{
private:
    /* data */
    std::unordered_multimap<const std::type_info*,Component*> components;
    std::vector<EntityGroup*> parents;
public:
    uint64_t id;
    template <typename T> void add_component(const char* lua_type = nullptr);
    void remove_component(Component*);
    void add_to_group(EntityGroup*);
    void remove_from_group(EntityGroup*);
    std::vector<EntityGroup*> get_parent_groups();
    template <typename T> T* get_component(const char* lua_type = nullptr);
    Component* get_component(const char*);
    template <typename T> T* require_component();
    template <typename T> bool has_component(const char* lua_type = nullptr);
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
