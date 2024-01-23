template <typename T> void Component::push_data(const char *name, void *data)
{
    this->component_data.emplace_back(name, std::pair<const type_info*, void*>(&typeid(T), data));
}

template <typename T> inline T* Entity::get_component()
{
    if(this->has_component<T>())
    {
        return (T*)this->components[&typeid(T)];
    }
    else
    {
        return nullptr;
    }
}
template <typename T> inline bool Entity::has_component()
{
    return this->components.find(&typeid(T)) != this->components.end();
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

template<typename T> inline void Entity::add_component()
{
    // Insert the component into the components map
    if(!has_component<T>())
    {
        T* component = new T();
        component->init(this);
        std::pair<const std::type_info *, Component *> pair(&typeid(T), component);
        this->components.insert(pair);
    }
    else
    {
        print_warn("Entity already has component of type " + std::string(typeid(T).name()) + "! Component not added.");
    }
}

template<typename T> inline void Component::register_component()
{
    Component::component_types[typeid(T).name()] = []() -> Component* { return new T(); };
}

template<typename T> inline void System::register_system()
{
    System::system_types[typeid(T).name()] = []() -> System* { return new T(); };
}