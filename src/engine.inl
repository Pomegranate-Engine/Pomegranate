template <typename T> inline T* Entity::get_component()
{
    for (auto & component : this->components)
    {
        if (typeid(*(T*)component).name() == typeid(T).name())
        {
            return dynamic_cast<T*>(component);
        }
    }
    return nullptr;
}
template <typename T> inline bool Entity::has_component() {
    for (auto & component : this->components)
    {
        if (typeid(*(T*)component).name() == typeid(T).name())
        {
            return true;
        }
    }
    return false;
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