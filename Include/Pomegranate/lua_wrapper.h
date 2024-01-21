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
private:
    /* data */
    lua_State* lua;
public:
    LuaSystem(const char* txt);
    ~LuaSystem();
    void init(Entity* e) override;
    void pre_tick() override;
    void tick(Entity* e) override;
    void post_tick() override;
    void pre_draw() override;
    void draw(Entity* e) override;
    void post_draw() override;
};

static int lua_print_log(lua_State* l);
static int lua_print_pass(lua_State* l);
static int lua_print_fail(lua_State* l);
static int lua_print_error(lua_State* l);
static int lua_print_warn(lua_State* l);
static int lua_print_info(lua_State* l);
static int lua_print_debug(lua_State* l);
static int lua_print_notice(lua_State* l);
static int lua_print_ready(lua_State* l);
static int lua_print_assert(lua_State* l);

static int lua_window_new(lua_State* l);
static int lua_window_get_width(lua_State* l);
static int lua_window_get_height(lua_State* l);
static int lua_window_open(lua_State* l);
static int lua_window_close(lua_State* l);
static int lua_window_make_current(lua_State* l);

static int lua_vec2_new(lua_State* l);
static int lua_vec2_get_x(lua_State *L);
static int lua_vec2_get_y(lua_State *L);
static int lua_vec2_set_x(lua_State *L);
static int lua_vec2_set_y(lua_State *L);
static int lua_vec2_index(lua_State *L);
static int lua_vec2_newindex(lua_State *L);

void lua_wrapper_init(lua_State* l);

#endif //POMEGRANATE_ENGINE_LUA_WRAPPER_H
