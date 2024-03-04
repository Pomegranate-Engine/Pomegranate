class CameraControllerSystem : public System
{
public:
    CameraControllerSystem()
    {
        register_system<CameraControllerSystem>();
    }
    void tick(Entity* entity) override
    {
        if(use_lua_camera_controller)
        {
            if (entity->has_component<LuaComponent>("CameraController") && entity->has_component<Transform>()) {
                auto *c = entity->get_component<LuaComponent>("CameraController");
                auto *t = entity->get_component<Transform>();
                auto vx = c->get<double>("vx");
                auto vy = c->get<double>("vy");
                Vec2 v = Vec2((float) vx, (float) vy);
                Vec2 m = Vec2(0, 0);
                if (InputManager::get_key(SDL_SCANCODE_W)) {
                    m.y -= 1;
                }
                if (InputManager::get_key(SDL_SCANCODE_S)) {
                    m.y += 1;
                }
                if (InputManager::get_key(SDL_SCANCODE_A)) {
                    m.x -= 1;
                }
                if (InputManager::get_key(SDL_SCANCODE_D)) {
                    m.x += 1;
                }

                auto speed = c->get<double>("speed");
                auto drag = c->get<double>("drag");
                v += m * (float) speed;
                v *= (float) drag;


                c->set<double>("vx", v.x);
                c->set<double>("vy", v.y);

                t->pos += v;
            }
        }
        else
        {
            if (entity->has_component<CameraController>() && entity->has_component<Transform>()) {
                auto *c = entity->get_component<CameraController>();
                auto *t = entity->get_component<Transform>();
                auto vx = c->velocity.x;
                auto vy = c->velocity.y;
                Vec2 v = Vec2((float) vx, (float) vy);
                Vec2 m = Vec2(0, 0);
                if (InputManager::get_key(SDL_SCANCODE_W)) {
                    m.y -= 1;
                }
                if (InputManager::get_key(SDL_SCANCODE_S)) {
                    m.y += 1;
                }
                if (InputManager::get_key(SDL_SCANCODE_A)) {
                    m.x -= 1;
                }
                if (InputManager::get_key(SDL_SCANCODE_D)) {
                    m.x += 1;
                }

                auto speed = c->speed;
                auto drag = c->drag;
                v += m * (float) speed;
                v *= (float) drag;


                c->velocity.x = v.x;
                c->velocity.y = v.y;

                t->pos += v;
            }
        }
    }
};

//Example system that allows you to draw the physics objects
class Drag : public System
{
public:
    Entity* currently_dragged = nullptr;
    bool clicked = false;
    Drag()
    {
        register_system<Drag>();
    }
    void pre_tick() override
    {
        Vec2 mousepos = InputManager::get_mouse_position() + Camera::current->get_component<Transform>()->pos;
        if (currently_dragged != nullptr)
        {
            auto *p = currently_dragged->get_component<PhysicsObject>();
            auto *t = currently_dragged->get_component<Transform>();
            p->set_velocity(t->pos.direction_to(mousepos) * 5.0);

            if (currently_dragged->has_component<DestroyAfterTime>())
            {
                auto *d = currently_dragged->get_component<DestroyAfterTime>();
                d->time = 0.0;
            }
        }
        if (!InputManager::get_mouse_button(SDL_BUTTON_LEFT))
        {
            if (currently_dragged != nullptr)
            {
                currently_dragged->get_component<PhysicsObject>()->drag -= 5.0;
            }
            currently_dragged = nullptr;
        }
    }
    void tick(Entity* entity) override
    {
        if(entity != nullptr)
        {
            Vec2 mousepos = InputManager::get_mouse_position() + Camera::current->get_component<Transform>()->pos;
            if (currently_dragged == nullptr)
            {
                if (entity->has_component<PhysicsObject>())
                {
                    auto *p = entity->get_component<PhysicsObject>();
                    auto *t = entity->get_component<Transform>();
                    auto *c = entity->get_component<CollisionShape>();
                    if (p->body_type == PHYSICS_BODY_TYPE_RIGID)
                    {
                        if (mousepos.distance_to(t->pos) < c->radius * t->scale.x)
                        {
                            if (InputManager::get_mouse_button(SDL_BUTTON_LEFT))
                            {
                                entity->get_ref(currently_dragged);
                                currently_dragged->get_component<PhysicsObject>()->drag += 5.0;
                            }
                        }
                    }
                }
            }
        }
    }
};

class Spawn : public System
{
public:
    bool clicked = false;
    Spawn()
    {
        register_system<Spawn>();
    }
    void pre_tick() override
    {
        Vec2 mousepos = InputManager::get_mouse_position() + Camera::current->get_component<Transform>()->pos;
        if (InputManager::get_mouse_button(SDL_BUTTON_RIGHT))
        {
            if (!clicked)
            {
                auto *e = new Entity();
                e->add_component<PhysicsObject,CollisionShape,Transform,Sprite>();
                e->get_component<Transform>()->pos = mousepos;
                float ransize = (float)rand()/RAND_MAX*0.2f+0.05f;
                e->get_component<Transform>()->scale = Vec2(ransize,ransize);
                auto *s = e->get_component<Sprite>();
                s->load_texture("res/pomegranate.png");
                s->color = Color((float)rand()/RAND_MAX*360.0f, 1.0);
                e->get_component<PhysicsObject>()->mass = ransize;
                auto *c = e->get_component<CollisionShape>();
                c->radius = 128.0;
                c->restitution = 0.0;

                /*e->add_component<DestroyAfterTime>();
                auto *d = e->get_component<DestroyAfterTime>();
                d->length = 20.0;
                d->time = 0.0;
                */
                EntityGroup::get_group("PHYSICS")->add_entity(e);
                //clicked = true;
            }
        } else
        {
            clicked = false;
        }
    }
};

class Destroy : public System
{
public:
    Destroy()
    {
        register_system<Destroy>();
    }
    void tick(Entity* entity) override
    {
        if(entity->has_component<DestroyAfterTime>())
        {
            auto* d = entity->get_component<DestroyAfterTime>();
            d->time += 0.016f;
            if(d->time >= d->length)
            {
                entity->destroy();
            }
        }
    }
};