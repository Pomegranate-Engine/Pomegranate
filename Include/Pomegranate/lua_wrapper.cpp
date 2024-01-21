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

static const struct luaL_Reg pomegranate_window_methods[] =
{
    {"get_width", lua_window_get_width},
    {"get_height", lua_window_get_height},
    {"open", lua_window_open},
    {"close", lua_window_close},
    {"make_current", lua_window_make_current},
    {nullptr, nullptr}
};

static const struct luaL_Reg pomegranate_vec2_meta[] =
{
    {"__index", lua_vec2_index},
    {"__newindex", lua_vec2_newindex},
    {nullptr, nullptr}
};

static const struct luaL_Reg pomegranate[] =
{
    {"print_log", lua_print_log},
    {"print_pass", lua_print_pass},
    {"print_fail", lua_print_fail},
    {"print_error", lua_print_error},
    {"print_warn", lua_print_warn},
    {"print_info", lua_print_info},
    {"print_debug", lua_print_debug},
    {"print_notice", lua_print_notice},
    {"print_ready", lua_print_ready},
    {"print_assert", lua_print_assert},
    {"new_window", lua_window_new},
    {"new_vec2", lua_vec2_new},
    {nullptr, nullptr}
};
void lua_wrapper_init(lua_State* l)
{
    luaL_newmetatable(l, "Pomegranate.Window");
    luaL_setfuncs(l, pomegranate_window_methods, 0);

    lua_pushvalue(l,-1);
    lua_setfield(l,-2,"__index");

    luaL_newmetatable(l, "Pomegranate.Vec2");
    luaL_setfuncs(l, pomegranate_vec2_meta, 0);
    lua_pop(l, 1);  // Pop the metatable

    lua_pushvalue(l,-1);
    lua_setfield(l,-2,"__index");

    lua_newtable(l);
    lua_setfield(l, -2, "__metatable");

    luaL_register(l, "Pomegranate", pomegranate);
}

LuaSystem::LuaSystem(const char* txt)
{
    this->lua = luaL_newstate();
    lua_wrapper_init(this->lua);
    if(luaL_dofile(this->lua, txt))
    {
        print_error("cannot run configuration file: " + std::string(lua_tostring(this->lua, -1)));
    }

}

LuaSystem::~LuaSystem()
{
    lua_close(this->lua);
}

void LuaSystem::init(Entity *e)
{
    lua_getglobal(this->lua, "init");

    if (lua_isfunction(this->lua, -1))
    {
        lua_pcall(this->lua, 0, 0, 0);
    }
    else
    {
        // Handle the case when the function is not found
        fprintf(stderr, "Error: Lua function '%s' not found\n", "init");
        lua_pop(this->lua, 1);  // Remove the non-function value from the stack
    }
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
