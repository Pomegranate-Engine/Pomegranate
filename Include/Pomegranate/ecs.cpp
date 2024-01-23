#include "ecs.h"
#include <utility>

std::vector<System*> System::global_systems = std::vector<System*>();
std::vector<Entity*> Entity::entities = std::vector<Entity*>();
std::map<const char*, std::function<Component*()>> Component::component_types = std::map<const char*, std::function<Component*()>>();
std::map<const char*, std::function<System*()>> System::system_types = std::map<const char*, std::function<System*()>>();

Entity::Entity()
{
    Entity::entities.push_back(this);
    this->id = Entity::entity_count++;
    this->components = std::map<const std::type_info*,Component*>();
}

uint64_t Entity::get_id() const
{
    return this->id;
}
Component* Entity::get_component(const char* name)
{
    for (auto c : components)
    {
        if (std::string(c.first->name()) == "class " + std::string(name))
        {
            return c.second;
        }
    }
    return nullptr;
}

bool Entity::has_component(const char * name)
{
    for (auto c : components)
    {
        if (std::string(c.first->name()) == "class " + std::string(name))
        {
            return true;
        }
    }
    return false;
}

Entity::~Entity()
{
    for (int i = 0; i < Entity::entities.size(); i++)
    {
        if(Entity::entities[i] == this)
        {
            Entity::entities.erase(Entity::entities.begin() + i);
        }
    }
}


void Entity::remove_component(Component* component)
{
    //this->components.erase(std::remove(this->components.begin(), this->components.end(), component), this->components.end());
}

uint64_t Entity::entity_count = 0;


System::System() = default;

System::~System() = default;

void System::init(Entity*) {}
void System::tick(Entity*) {}
void System::draw(Entity*) {}
void System::pre_tick(){}
void System::pre_draw(){}
void System::post_tick(){}
void System::post_draw(){}

void System::global_system_tick()
{
    for(auto & system : System::global_systems)
    {
        system->pre_tick();
        for(auto & entity : Entity::entities)
        {
            system->tick(entity);
        }
        system->post_tick();
    }
}

void System::add_global_system(System * system)
{
    System::global_systems.push_back(system);
}

void System::remove_global_system(System * system)
{
    for (int i = 0; i < System::global_systems.size(); ++i)
    {
        if (System::global_systems[i] == system)
        {
            System::global_systems.erase(System::global_systems.begin() + i);
            return;
        }
    }
}

void System::global_system_draw(std::function<bool(Entity*, Entity*)> sortingFunction)
{
    // Sort entities using the provided sorting function
    std::sort(Entity::entities.begin(), Entity::entities.end(), std::move(sortingFunction));

    for (auto& system : System::global_systems)
    {
        system->pre_draw();
        for (auto& entity : Entity::entities)
        {
            system->draw(entity);
        }
        system->post_draw();
    }
}

EntityGroup::EntityGroup()
{
    this->entities = std::vector<Entity*>();
    this->systems = std::vector<System*>();
    this->child_groups = std::vector<EntityGroup>();
}

EntityGroup::~EntityGroup()
= default;

void EntityGroup::add_entity(Entity* entity)
{
    entity->add_to_group(this);
    this->entities.push_back(entity);
}

void EntityGroup::remove_entity(Entity* entity)
{
    for (auto & entitie : entities)
    {
        if (entitie->get_id() == entity->get_id())
        {
            entitie->remove_from_group(this);
            return;
        }
    }
    for(auto & child_group : this->child_groups)
    {
        child_group.remove_entity(entity);
    }
}

void EntityGroup::add_system(System * system)
{
    this->systems.push_back(system);
}

void EntityGroup::remove_system(System * system)
{

}

void EntityGroup::add_group(const EntityGroup& entityGroup)
{
    this->child_groups.push_back(entityGroup);
}

void EntityGroup::remove_group(const EntityGroup& entityGroup)
{

}

void EntityGroup::tick()
{
    for(auto & system : this->systems)
    {
        system->pre_tick();
        for(auto & entitie : this->entities)
        {
            system->tick(entitie);
        }
        system->post_tick();
    }
//#pragma omp parallel for
    for(auto & child_group : this->child_groups)
    {
        child_group.tick();
    }
//#pragma omp barrier
}

void EntityGroup::draw(const std::function<bool(Entity*, Entity*)>& sortingFunction)
{
    // Sort entities using the provided sorting function
    std::sort(this->entities.begin(), this->entities.end(), sortingFunction);

    for(auto & system : this->systems)
    {
        system->pre_draw();
        for(auto & entity : this->entities)
        {
            system->draw(entity);
        }
        system->post_draw();
    }
    for(auto & group : this->child_groups)
    {
        group.draw(sortingFunction);
    }
}

void Entity::add_to_group(EntityGroup * group)
{
    this->parents.push_back(group);
}

void Entity::remove_from_group(EntityGroup * group)
{
    for (int i = 0; i < this->parents.size(); ++i)
    {
        if (this->parents[i] == group)
        {
            this->parents.erase(this->parents.begin() + i);
            return;
        }
    }
}

std::vector<EntityGroup*> Entity::get_parent_groups()
{
    return this->parents;
}

void Component::init(Entity *)
{

}


