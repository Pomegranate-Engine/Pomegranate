#include "lua_wrapper.h"

static int lua_print_log(lua_State* l)
{
    const char* c = lua_tostring(l,1);
    print_log(std::string(c));
    return 0;
}
static int lua_print_pass(lua_State* l)
{
    const char* c = lua_tostring(l,1);
    print_pass(std::string(c));
    return 0;
}
static int lua_print_fail(lua_State* l)
{
    const char* c = lua_tostring(l,1);
    print_fail(std::string(c));
    return 0;
}
static int lua_print_error(lua_State* l)
{
    const char* c = lua_tostring(l,1);
    print_error(std::string(c));
    return 0;
}
static int lua_print_warn(lua_State* l)
{
    const char* c = lua_tostring(l,1);
    print_warn(std::string(c));
    return 0;
}
static int lua_print_info(lua_State* l)
{
    const char* c = lua_tostring(l,1);
    print_info(std::string(c));
    return 0;
}
static int lua_print_debug(lua_State* l)
{
    const char* c = lua_tostring(l,1);
    print_debug(std::string(c));
    return 0;
}
static int lua_print_notice(lua_State* l)
{
    const char* c = lua_tostring(l,1);
    print_notice(std::string(c));
    return 0;
}
static int lua_print_ready(lua_State* l)
{
    const char* c = lua_tostring(l,1);
    print_ready(std::string(c));
    return 0;
}
static int lua_print_assert(lua_State* l)
{
    const char* c = lua_tostring(l,1);
    print_assert(std::string(c));
    return 0;
}

static int lua_window_new(lua_State* l)
{
    const char* c = lua_tostring(l,1);
    int width = (int)lua_tonumber(l,2);
    int height = (int)lua_tonumber(l,3);
    auto* w = new Window(c,width,height);
    lua_pushlightuserdata(l, w);

    luaL_getmetatable(l, "pomegranate.window");
    lua_setmetatable(l, -2);

    return 1;
}

static int lua_window_get_width(lua_State* l)
{
    auto* w = (Window*)lua_touserdata(l,1);
    lua_pushnumber(l, w->get_width());
    return 1;
}

static int lua_window_get_height(lua_State* l)
{
    auto* w = (Window*)lua_touserdata(l,1);
    lua_pushnumber(l, w->get_height());
    return 1;
}

static int lua_window_open(lua_State* l)
{
    auto* w = (Window*)lua_touserdata(l,1);
    w->open();
    return 0;
}

static int lua_window_close(lua_State* l)
{
    auto* w = (Window*)lua_touserdata(l,1);
    w->close();
    return 0;
}

static int lua_window_make_current(lua_State* l)
{
    auto* w = (Window*)lua_touserdata(l,1);
    w->make_current();
    return 0;
}

static int lua_vec2_new(lua_State* l)
{
    Vec2 *v = (Vec2 *)lua_newuserdata(l, sizeof(Vec2));
    v->x = (float)luaL_optnumber(l, 1, 0.0);
    v->y = (float)luaL_optnumber(l, 2, 0.0);

    luaL_getmetatable(l, "Pomegranate.Vec2");
    lua_setmetatable(l, -2);

    return 1;
}

static int lua_vec2_get_x(lua_State *L)
{
    Vec2 *v = (Vec2 *)luaL_checkudata(L, 1, "Pomegranate.Vec2");
    lua_pushnumber(L, v->x);
    return 1;
}

static int lua_vec2_get_y(lua_State *L)
{
    Vec2 *v = (Vec2 *)luaL_checkudata(L, 1, "Pomegranate.Vec2");
    lua_pushnumber(L, v->y);
    return 1;
}

static int lua_vec2_set_x(lua_State *L)
{
    Vec2 *v = (Vec2 *)luaL_checkudata(L, 1, "Pomegranate.Vec2");
    v->x = (float)luaL_checknumber(L, 2);
    return 0;
}

static int lua_vec2_set_y(lua_State *L)
{
    Vec2 *v = (Vec2 *)luaL_checkudata(L, 1, "Pomegranate.Vec2");
    v->y = (float)luaL_checknumber(L, 2);
    return 0;
}

static int lua_vec2_index(lua_State *L)
{
    const char *key = luaL_checkstring(L, 2);

    if (strcmp(key, "x") == 0) {
        lua_vec2_get_x(L);
    } else if (strcmp(key, "y") == 0) {
        lua_vec2_get_y(L);
    } else {
        lua_pushnil(L);
    }

    return 1;
}

static int lua_vec2_newindex(lua_State *L)
{
    const char *key = luaL_checkstring(L, 2);
    Vec2 *v = (Vec2 *)luaL_checkudata(L, 1, "Pomegranate.Vec2");

    if (strcmp(key, "x") == 0) {
        lua_vec2_set_x(L);
    } else if (strcmp(key, "y") == 0) {
        lua_vec2_set_y(L);
    } else {
        luaL_error(L, "Invalid field '%s'", key);
    }

    return 0;
}

void lua_wrapper_init(lua_State* l)
{

}

LuaSystem::LuaSystem(const char* txt)
{

}

LuaSystem::~LuaSystem()
{
    lua_close(this->lua);
}

void LuaSystem::init(Entity *e)
{
    //Open lua
    this->lua = luaL_newstate();
}

void LuaSystem::pre_tick()
{

}

void LuaSystem::tick(Entity *e)
{

}

void LuaSystem::post_tick()
{

}

void LuaSystem::pre_draw()
{

}

void LuaSystem::draw(Entity *e)
{

}

void LuaSystem::post_draw()
{

}
