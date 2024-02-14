namespace Pomegranate
{
    template <typename T> void Component::push_data(std::string name, void *data)
    {
        auto p = std::pair<const std::type_info*, void*>(&typeid(T), data);
        this->component_data[name] = p;
    }
    template <typename T> T Component::get(std::string name)
    {
        if(this->component_data[name].second != nullptr)
        {
            return *(T*)this->component_data[name].second;
        }
        else
        {
            print_warn("Component " + std::string(typeid(T).name()) + " does not have data " + std::string(name) + "! Returning default value.");
            return T();
        }
    }
    template <typename T> void Component::set(const char *name, T value)
    {
        *(T*)this->component_data[name].second = value;
    }

    template <typename T> inline T* Entity::get_component(const char* lua_type)
    {
        if(typeid(T) == typeid(LuaComponent))
        {
            auto r = this->components.equal_range(&typeid(T));
            for(auto i = r.first; i != r.second; i++)
            {
                if(((LuaComponent*)(i->second))->real_type == LuaComponent::lua_component_types[lua_type])
                {
                    return (T*)i->second;
                }
            }
            return nullptr;
        }
        else
        {
            if(this->has_component<T>())
            {
                T* component = (T*)this->components.equal_range(&typeid(T)).first->second;
                return component;
            }
            else
            {
                return nullptr;
            }
        }
    }
    template <typename T> inline bool Entity::has_component(const char* lua_type)
    {
        if(typeid(T) == typeid(LuaComponent))
        {
            auto r = this->components.equal_range(&typeid(T));
            for(auto i = r.first; i != r.second; i++)
            {
                if(((LuaComponent*)(i->second))->real_type == LuaComponent::lua_component_types[lua_type])
                {
                    return true;
                }
            }
            return false;
        }
        else
        {
            return this->components.find(&typeid(T)) != this->components.end();
        }
    }

    template<typename T> inline T *Entity::require_component()
    {
        if(this->has_component<T>())
        {
            return this->get_component<T>();
        }
        else
        {
            this->add_component<T>();
            return this->get_component<T>();
        }
    }

    template<typename T> T* Entity::add_component(const char* lua_type)
    {
        if(typeid(T) == typeid(LuaComponent))
        {
            if(!has_component<T>())
            {
                LuaComponent* component = new LuaComponent();
                component->real_type = LuaComponent::lua_component_types[lua_type];
                component->load_script(lua_type);
                component->init(this);
                std::pair<const std::type_info *, Component *> pair(&typeid(T), component);
                this->components.insert(pair);
                return (T*)component;
            }
            else
            {
                print_warn("Entity already has component of type " + std::string(typeid(T).name()) + "! Component not added.");
            }
        }
        else
        {
            if(!has_component<T>())
            {
                T* component = new T();
                component->init(this);
                std::pair<const std::type_info *, Component *> pair(&typeid(T), component);
                this->components.insert(pair);
                return (T*)component;
            }
            else
            {
                print_warn("Entity already has component of type " + std::string(typeid(T).name()) + "! Component not added.");
                return get_component<T>();
            }
        }
        return nullptr;
    }

    template<typename T> inline void Component::register_component()
    {
        Component::component_types[typeid(T).name()] = []() -> Component* { return new T(); };
    }

    template<typename T> inline void System::register_system()
    {
        System::system_types[typeid(T).name()] = []() -> System* { return new T(); };
    }
}