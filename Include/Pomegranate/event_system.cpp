#include "event_system.h"
namespace Pomegranate
{
    Event::Event()
    {
        this->callbacks = std::vector<std::pair<Entity*,std::function<void(Entity*)>>>();
    }

    void Event::trigger()
    {
        for (auto c : this->callbacks)
        {
            c.second(c.first);
        }
    }

    void Event::add_listener(std::function<void(Entity *)> callback)
    {

    }

    void Event::remove_listener(std::function<void(Entity *)> callback)
    {

    }
}
