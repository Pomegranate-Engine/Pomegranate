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
template <typename T> inline bool Entity::has_component() {
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
        auto* component = new T();
        this->add_component(component);
        return component;
    }
}