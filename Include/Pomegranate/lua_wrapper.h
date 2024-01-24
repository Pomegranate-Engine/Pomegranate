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

class LuaSystem : public System
{
public:
    lua_State* state;
    bool loaded = false;
    LuaSystem();
    void load_script(const char* path);
    void init(Entity* entity) override;
    void tick(Entity* entity) override;
    void draw(Entity* entity) override;
    void pre_tick() override;
    void post_tick() override;
    void pre_draw() override;
    void post_draw() override;
};

//Lua tools
void lua_push_component(Component* component,lua_State* l);
void lua_push_vec2(Vec2* vec2,lua_State* l);

//Lua gets
int lua_get_component(lua_State* l);
int lua_has_component(lua_State* l);
int lua_get_key(lua_State* l);
int lua_get_axis(lua_State* l);
int lua_get_mouse(lua_State* l);
int lua_get_mouse_pos(lua_State* l);

//Lua functions
int lua_debug_draw_rect(lua_State* l);

int lia_print_pass(lua_State* l);
int lia_print_fail(lua_State* l);
int lia_print_error(lua_State* l);
int lia_print_warn(lua_State* l);
int lia_print_info(lua_State* l);
int lia_print_debug(lua_State* l);
int lia_print_notice(lua_State* l);
int lia_print_log(lua_State* l);
int lia_print_ready(lua_State* l);
int lia_print_assert(lua_State* l);

//Wrapper
void clean_refs(lua_State* l);
extern std::map<Component*,int> ref_map;

#endif //POMEGRANATE_ENGINE_LUA_WRAPPER_H
