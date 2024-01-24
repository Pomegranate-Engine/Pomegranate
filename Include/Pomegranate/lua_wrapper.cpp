#include "lua_wrapper.h"

std::map<Component*,int> ref_map;

void lua_push_vec2(Vec2* vec2,lua_State* l)
{
    lua_newtable(l);
    lua_pushstring(l, "x");
    lua_pushnumber(l, vec2->x);
    lua_settable(l, -3);
    lua_pushstring(l, "y");
    lua_pushnumber(l, vec2->y);
    lua_settable(l, -3);
}

void lua_push_component(Component* component,lua_State* l)
{
    if(ref_map.find(component) != ref_map.end())
    {
        lua_rawgeti(l, LUA_REGISTRYINDEX, ref_map[component]);
        return;
    }
    //Create a table
    lua_newtable(l);
    for (auto d : component->component_data)
    {
        //Push the name of the variable
        lua_pushstring(l, d.first);
        //Push the value of the variable
        if (d.second.first == &typeid(int))
        {
            lua_pushnumber(l, *(int*)d.second.second);
        }
        else if (d.second.first == &typeid(float))
        {
            lua_pushnumber(l, *(float*)d.second.second);
        }
        else if (d.second.first == &typeid(double))
        {
            lua_pushnumber(l, *(double*)d.second.second);
        }
        else if (d.second.first == &typeid(bool))
        {
            lua_pushboolean(l, *(bool*)d.second.second);
        }
        else if (d.second.first == &typeid(std::string))
        {
            lua_pushstring(l, ((std::string*)d.second.second)->c_str());
        }
        else if(d.second.first == &typeid(Vec2))
        {
            lua_push_vec2((Vec2*)d.second.second, l);
        }
        else if(d.second.first == &typeid(Vec2i*))
        {
            lua_push_vec2((Vec2*)d.second.second, l);
        }
        else
        {
            lua_pushnil(l);
        }
        //Set the value of the variable
        lua_settable(l, -3);
    }
    int ref = luaL_ref(l, LUA_REGISTRYINDEX);
    ref_map[component] = ref;
    lua_rawgeti(l, LUA_REGISTRYINDEX, ref);
}

int lua_get_component(lua_State* l)
{
    double d = lua_tonumber(l, 1);
    Entity* e = Entity::entities[(int)d];
    const char* name = lua_tostring(l, 2);
    Component* c = e->get_component(name);
    lua_pop(l, 2);
    lua_push_component(c, l);

    return 1;
}

int lua_has_component(lua_State* l)
{
    double d = lua_tonumber(l, 1);
    Entity* e = Entity::entities[(int)d];
    const char* name = lua_tostring(l, 2);
    bool has = e->has_component(name);
    lua_pushboolean(l, has);
    return 1;
}


int lua_get_key(lua_State* l)
{
    const char* name = lua_tostring(l, 1);
    auto key = static_cast<SDL_KeyCode>(SDL_GetKeyFromName(name));
    lua_pushboolean(l, InputManager::get_key(key));
    return 1;
}

int lua_get_axis(lua_State* l)
{
    const char* name1 = lua_tostring(l, 1);
    auto key1 = static_cast<SDL_Scancode>(SDL_GetScancodeFromName(name1));
    const char* name2 = lua_tostring(l, 2);
    auto key2 = static_cast<SDL_Scancode>(SDL_GetScancodeFromName(name2));
    float axis = 0;
    if(InputManager::get_key(key1))
    {
        axis -= 1;
    }
    if(InputManager::get_key(key2))
    {
        axis += 1;
    }
    lua_pushnumber(l, axis);
    return 1;
}

int lua_get_mouse(lua_State* l)
{
    int i = lua_tonumber(l, 1);
    lua_pushboolean(l, InputManager::get_mouse_button(i));
    return 1;
}

int lua_get_mouse_pos(lua_State* l)
{
    Vec2 pos = InputManager::get_mouse_position();
    lua_push_vec2(&pos, l);
    return 1;
}

int lua_debug_draw_rect(lua_State* l)
{
    //Read vector2 table position arg 1
    lua_pushstring(l, "x");
    lua_gettable(l, 1);
    double x = lua_tonumber(l, -1);
    lua_pushstring(l, "y");
    lua_gettable(l, 1);
    double y = lua_tonumber(l, -1);
    //Read vector2 table size arg 2
    lua_pushstring(l, "x");
    lua_gettable(l, 2);
    double w = lua_tonumber(l, -1);
    lua_pushstring(l, "y");
    lua_gettable(l, 2);
    double h = lua_tonumber(l, -1);

    //Read color arg 3
    lua_pushstring(l, "r");
    lua_gettable(l, 3);
    double r = lua_tonumber(l, -1);
    lua_pushstring(l, "g");
    lua_gettable(l, 3);
    double g = lua_tonumber(l, -1);
    lua_pushstring(l, "b");
    lua_gettable(l, 3);
    double b = lua_tonumber(l, -1);
    lua_pushstring(l, "a");
    lua_gettable(l, 3);
    double a = lua_tonumber(l, -1);

    //Set draw color
    SDL_SetRenderDrawColor(Window::current->get_sdl_renderer(), (uint8_t)r, (uint8_t)g, (uint8_t)b, (uint8_t)a);
    SDL_RenderRect(Window::current->get_sdl_renderer(), new SDL_FRect{(float)x, (float)y, (float)w, (float)h});
    return 0;
}

int lia_print_pass(lua_State* l)
{
    const char* str = lua_tostring(l, 1);
    print_pass(str);
    return 0;
}

int lia_print_fail(lua_State* l)
{
    const char* str = lua_tostring(l, 1);
    print_fail(str);
    return 0;
}

int lia_print_error(lua_State* l)
{
    const char* str = lua_tostring(l, 1);
    print_error(str);
    return 0;
}

int lia_print_warn(lua_State* l)
{
    const char* str = lua_tostring(l, 1);
    print_warn(str);
    return 0;
}

int lia_print_info(lua_State* l)
{
    const char* str = lua_tostring(l, 1);
    print_info(str);
    return 0;
}

int lia_print_debug(lua_State* l)
{
    const char* str = lua_tostring(l, 1);
    print_debug(str);
    return 0;
}

int lia_print_notice(lua_State* l)
{
    const char* str = lua_tostring(l, 1);
    print_notice(str);
    return 0;
}

int lia_print_log(lua_State* l)
{
    const char* str = lua_tostring(l, 1);
    print_log(str);
    return 0;
}

int lia_print_ready(lua_State* l)
{
    const char* str = lua_tostring(l, 1);
    print_ready(str);
    return 0;
}

int lia_print_assert(lua_State* l)
{
    const char* str = lua_tostring(l, 1);
    print_assert(str);
    return 0;
}

void clean_refs(lua_State* l)
{
    for(auto & ref : ref_map)
    {
        Component* c = ref.first;
        for (auto d : c->component_data)
        {
            lua_rawgeti(l, LUA_REGISTRYINDEX, ref.second);
            //Push the name of the variable
            lua_pushstring(l, d.first);
            //Get the variable
            lua_gettable(l, -2);
            //Check type
            if(d.second.first == &typeid(int))
            {
                *(int*)d.second.second = lua_tonumber(l, -1);
            }
            else if(d.second.first == &typeid(float))
            {
                *(float*)d.second.second = lua_tonumber(l, -1);
            }
            else if(d.second.first == &typeid(double))
            {
                *(double*)d.second.second = lua_tonumber(l, -1);
            }
            else if(d.second.first == &typeid(bool))
            {
                *(bool*)d.second.second = lua_toboolean(l, -1);
            }
            else if(d.second.first == &typeid(std::string))
            {
                *(std::string*)d.second.second = lua_tostring(l, -1);
            }
            else if(d.second.first == &typeid(Vec2))
            {
                Vec2* vec2 = (Vec2*)d.second.second;
                lua_pushstring(l, "x");
                lua_gettable(l, -2);
                vec2->x = lua_tonumber(l, -1);
                lua_pushstring(l, "y");
                lua_gettable(l, -3);
                vec2->y = lua_tonumber(l, -1);
            }
            else if(d.second.first == &typeid(Vec2i*))
            {
                Vec2i* vec2 = (Vec2i*)d.second.second;
                lua_pushstring(l, "x");
                lua_gettable(l, -2);
                vec2->x = lua_tonumber(l, -1);
                lua_pushstring(l, "y");
                lua_gettable(l, -3);
                vec2->y = lua_tonumber(l, -1);
            }
            else
            {
                print_error("Unknown type");
            }
            lua_pop(l, 1);
        }
        luaL_unref(l, LUA_REGISTRYINDEX, ref.second);

    }
    ref_map.clear();
}

void LuaSystem::init(Entity *entity)
{

}

void LuaSystem::tick(Entity *entity)
{
    if (!this->loaded) return;

    lua_State *l = this->state;
    lua_getglobal(l, "tick");
    if (!lua_isfunction(l, -1)) return;

    // Push entity as argument
    lua_pushnumber(l, (double)entity->get_id());

    int arguments = 1;  // Number of arguments pushed onto the stack

    int res = lua_pcall(l, arguments, 0, 0);

    // Clear the stack
    lua_pop(l, lua_gettop(l));

    // Print error if there is one
    if (res != LUA_OK)
    {
        print_error(lua_tostring(l, -1));
    }

    // Clean up references
    clean_refs(l);
}


void LuaSystem::draw(Entity *entity)
{

}

void LuaSystem::pre_tick()
{

}

void LuaSystem::post_tick()
{

}

void LuaSystem::pre_draw()
{

}

void LuaSystem::post_draw()
{

}

LuaSystem::LuaSystem()
{
    this->state = luaL_newstate();
    luaL_openlibs(state);
    //Add functions
    lua_register(state, "get_component", lua_get_component);
    lua_register(state, "has_component", lua_has_component);
    lua_register(state, "input_get_key", lua_get_key);
    lua_register(state, "input_get_axis", lua_get_axis);
    lua_register(state, "input_get_mouse", lua_get_mouse);
    lua_register(state, "input_get_mouse_pos", lua_get_mouse_pos);
    lua_register(state, "debug_draw_rect", lua_debug_draw_rect);
    lua_register(state, "print_pass", lia_print_pass);
    lua_register(state, "print_fail", lia_print_fail);
    lua_register(state, "print_error", lia_print_error);
    lua_register(state, "print_warn", lia_print_warn);
    lua_register(state, "print_info", lia_print_info);
    lua_register(state, "print_debug", lia_print_debug);
    lua_register(state, "print_notice", lia_print_notice);
    lua_register(state, "print_log", lia_print_log);
    lua_register(state, "print_ready", lia_print_ready);
    lua_register(state, "print_assert", lia_print_assert);
}

void LuaSystem::load_script(const char *path)
{
    luaL_dofile(this->state, path);
    this->loaded = true;
}
