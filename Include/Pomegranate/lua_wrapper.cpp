#include "lua_wrapper.h"

TestComponent::TestComponent()
{
    Component::register_component<TestComponent>();
    this->push_data("test_int", &typeid(int), &this->test_int);
    this->push_data("test_float", &typeid(float), &this->test_float);
    this->push_data("test_double", &typeid(double), &this->test_double);
    this->push_data("test_bool", &typeid(bool), &this->test_bool);
}

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
        else
        {
            lua_pushnil(l);
        }
        //Set the value of the variable
        lua_settable(l, -3);
    }
    //Save reference to the table
    int ref = luaL_ref(l, LUA_REGISTRYINDEX);
    //Push the reference to the table
    lua_pushnumber(l, ref);

}

int lua_get_entity(lua_State* l)
{
    double d = lua_tonumber(l, 1);
    Entity* e = Entity::entities[(int)d];
    //Create a table for the entity
    lua_newtable(l);
    //Push the entity id
    lua_pushstring(l, "id");
    lua_pushnumber(l, d);
    lua_settable(l, -3);
    return 1;
}

int lua_get_component(lua_State* l)
{
    double d = lua_tonumber(l, 1);
    Entity* e = Entity::entities[(int)d];
    const char* name = lua_tostring(l, 2);
    Component* c = e->get_component(name);
    lua_push_component(c, l);
    return 1;
}

void lua_wrapper_init(lua_State* l)
{
    luaL_openlibs(l);
    //Add functions
    lua_register(l, "get_entity", lua_get_entity);
    lua_register(l, "get_component", lua_get_component);
    luaL_dofile(l, "res/main.lua");
}
void lua_wrapper_tick(lua_State* l)
{
    lua_getglobal(l, "tick");
    lua_pcall(l, 0, 0, 0);
}