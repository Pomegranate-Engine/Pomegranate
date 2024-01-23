#include "lua_wrapper.h"

std::map<Component*,int> ref_map;

TestComponent::TestComponent()
{
    Component::register_component<TestComponent>();
    this->push_data<int>("test_int", &this->test_int);
    this->push_data<float>("test_float",  &this->test_float);
    this->push_data<double>("test_double",  &this->test_double);
    this->push_data<bool>("test_bool", &this->test_bool);
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
    ref_map.clear();
    lua_getglobal(l, "tick");
    int res = lua_pcall(l, 0, 0, 0);
    //Print error if there is one
    if (res != LUA_OK)
    {
        print_error(lua_tostring(l, -1));
    }
    //Iterate through ref_map
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
        }
        luaL_unref(l, LUA_REGISTRYINDEX, ref.second);
    }
}