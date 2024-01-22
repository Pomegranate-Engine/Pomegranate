#ifndef POMEGRANATE_ENGINE_LUA_WRAPPER_H
#define POMEGRANATE_ENGINE_LUA_WRAPPER_H
#include "engine.h"
#include "window.h"
#include "ecs.h"

extern "C"
{
#include <Lua/lua.h>
#include <Lua/lauxlib.h>
#include <Lua/lualib.h>
}

class TestComponent : public Component
{
public:
    int test_int = 5.0;
    float test_float = 0.0f;
    double test_double = 0.0;
    bool test_bool = false;
    TestComponent();
};

void lua_push_component(Component* component,lua_State* l);
void lua_push_vec2(Vec2* vec2,lua_State* l);
int lua_get_entity(lua_State* l);
int lua_get_component(lua_State* l);
void lua_wrapper_init(lua_State* l);
void lua_wrapper_tick(lua_State* l);

#endif //POMEGRANATE_ENGINE_LUA_WRAPPER_H
