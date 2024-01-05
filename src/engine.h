#ifndef ENGINE_H
#define ENGINE_H

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <iostream>
#include<string>
#include<vector>
#include<cstdio>
#include <typeinfo>

/*----------> RENDERING <----------*/

struct vec2
{
    float x;
    float y;

    vec2();
    vec2(float x, float y);
    vec2 operator+(const vec2& other) const;
    vec2 operator-(const vec2& other) const;
    vec2 operator*(const vec2& other) const;
    vec2 operator*(const float& other) const;
    vec2 operator/(const vec2& other) const;
    vec2 operator/(const float& other) const;
    vec2 operator+=(const vec2& other);
    vec2 operator-=(const vec2& other);
    vec2 operator*=(const vec2& other);
    vec2 operator*=(const float& other);
    vec2 operator/=(const vec2& other);
    vec2 operator/=(const float& other);
    bool operator==(const vec2& other) const;
    bool operator!=(const vec2& other) const;
    float dot(const vec2& other) const;
    float cross(const vec2& other) const;
    float length() const;
    vec2 normalized() const;
    vec2 rotate(float angle) const;
    vec2 lerp(const vec2& other, float t) const;
    vec2 slerp(const vec2& other, float t) const;
    vec2 direction_to(const vec2& other) const;
    float distance_to(const vec2& other) const;
    float angle_to(const vec2& other) const;
};

class Window
{
private:
    /* data */
    unsigned int width, height;
    const char* title;
    SDL_Window* window;
    SDL_Renderer* renderer;
public:

    Window(const char* title,int width, int height);
    ~Window();
    unsigned int get_width() const;
    unsigned int get_height() const;
    const char* get_title() const;
    SDL_Window* get_sdl_window() const;
    SDL_Renderer* get_sdl_renderer() const;
    int open();
    void close();
    void make_current();
    static Window* current;
};

/*----------> DEBUGGING <----------*/

void print_pass(std::string);
void print_fail(std::string);
void print_error(std::string);
void print_warn(std::string);
void print_info(std::string);
void print_debug(std::string);
void print_notice(std::string);
void print_log(std::string);
void print_ready(std::string);
void print_assert(std::string);

/*----------> ECS <----------*/

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
public:
    System();
    ~System();
    virtual void init(Entity*);
    virtual void tick(Entity*);
    virtual void draw(Entity*);
};
class Entity
{
private:
    /* data */
    std::vector<Component*> components;
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
class InputManager
{
private:
    /* data */
    static bool* keys;
    static bool* mouse_buttons;
    static vec2 mouse_position;
    static vec2 mouse_delta;
    static bool mouse_moved;
    static bool mouse_scrolled;
    static vec2 mouse_scroll;
public:
    InputManager();
    ~InputManager();
    static void init();
    static void update();
    static bool get_key(int);
    static bool get_mouse_button(int);
    static vec2 get_mouse_position();
    static vec2 get_mouse_delta();
    static bool get_mouse_moved();
    static bool get_mouse_scrolled();
    static vec2 get_mouse_scroll();
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

#include "engine.inl"

#endif // ENGINE_H