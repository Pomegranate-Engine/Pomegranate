#include "ecs.h"


Entity::Entity()
{
    Entity::entities.push_back(this);
    this->id = Entity::entity_count++;
    print_log("Added entity: " + std::to_string(this->id));
    this->components = std::map<const std::type_info*,Component*>();
}

uint64_t Entity::get_id() const {
    return this->id;
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


void Entity::add_component(Component* component)
{
    // Insert the component into the components map

    std::pair<const std::type_info*,Component*> pair(&typeid(*component),component);
    this->components.insert(pair);
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

EntityGroup::EntityGroup()
{
    this->entities = std::vector<Entity*>();
    this->systems = std::vector<System*>();
    this->child_groups = std::vector<EntityGroup>();
}

EntityGroup::~EntityGroup()
{

}

void EntityGroup::add_entity(Entity* entity)
{
    entity->add_to_group(this);
    this->entities.push_back(entity);
}

void EntityGroup::remove_entity(Entity* entity)
{
    for (int i = 0; i < entities.size(); ++i)
    {
        if (entities[i]->get_id() == entity->get_id())
        {
            entities[i]->remove_from_group(this);
            return;
        }
    }
    for(int i = 0; i < this->child_groups.size(); ++i)
    {
        this->child_groups[i].remove_entity(entity);
    }
}

void EntityGroup::add_system(System * system)
{
    this->systems.push_back(system);
}

void EntityGroup::remove_system(System * system)
{

}

void EntityGroup::add_group(EntityGroup entityGroup)
{
    this->child_groups.push_back(entityGroup);
}

void EntityGroup::remove_group(EntityGroup entityGroup)
{

}

void EntityGroup::tick()
{
    for(int i = 0; i < this->entities.size(); ++i)
    {
        for(auto & system : this->systems)
        {
            system->tick(this->entities[i]);
        }
    }
//#pragma omp parallel for
    for(int i = 0; i < this->child_groups.size(); ++i)
    {
        this->child_groups[i].tick();
    }
//#pragma omp barrier
}

void EntityGroup::draw()
{
    for(auto & entity : this->entities)
    {
        for(auto & system : this->systems)
        {
            system->draw(entity);
        }
    }
    for(auto & group : this->child_groups)
    {
        group.draw();
    }
}

std::vector<Entity*> Entity::entities = std::vector<Entity*>();

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
