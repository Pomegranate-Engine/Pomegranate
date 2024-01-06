#include <iostream>
#include<cstdint>
#include<vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "engine/pomegranate.h"
#include"engine/standard_ecs.h"
#include"engine/standard_ecs_rendering.h"
#include"engine/standard_ecs_audio.h"
#include <omp.h>

unsigned int FRAME;
float DELTA = 0.0;

class Drag : public System
{
    public:
    Entity* currently_dragged = nullptr;
    bool clicked = false;
    void tick(Entity* entity) override
    {
        vec2 mousepos = InputManager::get_mouse_position()+Camera::current->get_component<Transform>()->pos;
        if(currently_dragged== nullptr)
        {
            if (entity->has_component<PhysicsObject>() && entity->has_component<Transform>()) {
                auto *p = entity->get_component<PhysicsObject>();
                auto *t = entity->get_component<Transform>();
                auto *c = entity->get_component<CollisionShape>();
                if (p->body_type == PHYSICS_BODY_TYPE_RIGID) {
                    if (mousepos.distance_to(t->pos) < c->radius*t->scale.x) {
                        if (InputManager::get_mouse_button(SDL_BUTTON_LEFT))
                        {
                            currently_dragged = entity;
                            currently_dragged->get_component<PhysicsObject>()->drag*=5.0;
                        }
                    }
                }
            }
        }
        if(currently_dragged!= nullptr)
        {
            auto* p = currently_dragged->get_component<PhysicsObject>();
            auto* t = currently_dragged->get_component<Transform>();
            p->linear_velocity = t->pos.direction_to(mousepos) * t->pos.distance_to(mousepos) * 10.0;
            p->angular_velocity = 0.0;
        }
        if (!InputManager::get_mouse_button(SDL_BUTTON_LEFT)) {
            if(currently_dragged!= nullptr)
            {
                currently_dragged->get_component<PhysicsObject>()->drag/=5.0;
            }
            currently_dragged = nullptr;

        }
        if (InputManager::get_mouse_button(SDL_BUTTON_RIGHT)) {
            if(!clicked)
            {
                if (entity->has_component<PhysicsObject>() && entity->has_component<Transform>()) {
                    auto *p = entity->get_component<PhysicsObject>();
                    auto* t = entity->get_component<Transform>();
                    p->linear_velocity = t->pos.direction_to(mousepos) * 10000.0;
                }
            }
            clicked = true;
        }
        else
        {
            clicked = false;
        }
    }
};

Window main_window("Window", 1024, 720);

int main(int argc, char* argv[])
{
    print_log("Initializing Pomegranate");
    pomegranate_init();

    print_log("Opening Test Window");
    main_window.open();
    main_window.make_current();
    print_log("Window opened: " + std::string(main_window.get_title()) + " with resolution of " + std::to_string(main_window.get_width()) + "x" + std::to_string(main_window.get_height()));

    //Load test sound
    Sample sample = Sample("res/sound test.wav");

    //Create basic physics test scene
    EntityGroup group = EntityGroup();
    auto camera = new Entity();
    camera->add_component(new Camera());
    camera->add_component(new Transform());
    Camera::make_current(camera);
    group.add_entity(camera);
    for(int z = -0; z <= 0; z++)
    {
        for (int i = -0; i <= 0; i++) {
            auto* physics_body = new Entity();
            physics_body->add_component(new PhysicsObject());
            physics_body->add_component(new Transform());
            physics_body->get_component<Transform>()->scale = vec2(1.0,1.0);
            auto* s = new Sprite();
            s->load_texture("res/circle_none.png");
            physics_body->add_component(s);
            auto* c = new CollisionShape();
            c->radius = 16.0;
            c->restitution = 0.0;
            physics_body->add_component(c);
            physics_body->get_component<Transform>()->pos = vec2(256 + z*32, 16 + i * 32);
            print_log(std::to_string(physics_body->get_id()));
            group.add_entity(physics_body);
        }
    }

    for (int i = -32; i <= 32; ++i)
    {
        auto* collision_body = new Entity();
        collision_body->add_component(new CollisionShape());
        collision_body->add_component(new Transform());
        auto* p = new PhysicsObject();
        p->body_type = PHYSICS_BODY_TYPE_STATIC;
        collision_body->add_component(p);
        collision_body->get_component<Transform>()->pos = vec2(256+8*i, 256+196-(abs(i)*abs(i))*0.2);
        collision_body->add_component(new DebugCircle());
        group.add_entity(collision_body);
    }


    group.add_system(new Render());
    group.add_system(new RigidBody());
    group.add_system(new Drag());

    for (int i = 0; i < Entity::entities.size(); ++i) {
        Entity::entities[i]->id = i;
    }



    float tick_time = 0.0;
    bool is_running = true;
    SDL_Event event;

    while (is_running)
    {
        Uint64 start = SDL_GetPerformanceCounter();
        InputManager::update();
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT) {
                is_running = false;
            }
        }

        //- - - - - # UPDATE # - - - - -
        if (tick_time > 0.016)
        {
            tick_time = 0.0;
            group.tick();
            if(InputManager::get_key(SDL_SCANCODE_LEFT))
            {
                camera->get_component<Transform>()->pos.x -= 10.0;
            }
            if(InputManager::get_key(SDL_SCANCODE_RIGHT))
            {
                camera->get_component<Transform>()->pos.x += 10.0;
            }
            if(InputManager::get_key(SDL_SCANCODE_UP))
            {
                camera->get_component<Transform>()->pos.y -= 10.0;
            }
            if(InputManager::get_key(SDL_SCANCODE_DOWN))
            {
                camera->get_component<Transform>()->pos.y += 10.0;
            }
            if(InputManager::get_key(SDL_SCANCODE_SPACE))
            {
                sample.play();
            }
        }

        //- - - - - # DRAW # - - - - -
        SDL_SetRenderDrawColor(main_window.get_sdl_renderer(), 128, 128, 128, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(main_window.get_sdl_renderer());
        SDL_SetRenderDrawColor(main_window.get_sdl_renderer(), 255, 255, 255, 255);

        group.draw();

        SDL_RenderPresent(main_window.get_sdl_renderer());
        Uint64 end = SDL_GetPerformanceCounter();

        float secondsElapsed = (float)(end - start) / (float)SDL_GetPerformanceFrequency();
        DELTA = secondsElapsed;
        tick_time += DELTA;
    }

    pomegranate_quit();
    return 0;
}
