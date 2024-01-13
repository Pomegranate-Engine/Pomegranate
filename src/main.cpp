#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <Pomegranate/pomegranate.h>
#include <chrono>

//Example system that allows you to draw the physics objects
class Drag : public System
{
    public:
    Entity* currently_dragged = nullptr;
    bool clicked = false;
    void tick(Entity* entity) override
    {
        if(entity!=nullptr)
        {
            Vec2 mousepos = InputManager::get_mouse_position() + Camera::current->get_component<Transform>()->pos;
            if (currently_dragged == nullptr)
            {
                if (entity->has_component<PhysicsObject>() && entity->has_component<Transform>())
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
                                currently_dragged = entity;
                                currently_dragged->get_component<PhysicsObject>()->drag *= 5.0;
                            }
                        }
                    }
                }
            }
            if (currently_dragged != nullptr)
            {
                auto *p = currently_dragged->get_component<PhysicsObject>();
                auto *t = currently_dragged->get_component<Transform>();
                p->linear_velocity = t->pos.direction_to(mousepos) * t->pos.distance_to(mousepos) * 10.0;
                p->angular_velocity = 0.0;
            }
            if (!InputManager::get_mouse_button(SDL_BUTTON_LEFT))
            {
                if (currently_dragged != nullptr)
                {
                    currently_dragged->get_component<PhysicsObject>()->drag /= 5.0;
                }
                currently_dragged = nullptr;

            }
            if (InputManager::get_mouse_button(SDL_BUTTON_RIGHT))
            {
                if (!clicked)
                {
                    if (entity->has_component<PhysicsObject>() && entity->has_component<Transform>())
                    {
                        auto *p = entity->get_component<PhysicsObject>();
                        auto *t = entity->get_component<Transform>();
                        p->linear_velocity = t->pos.direction_to(mousepos) * 10000.0;
                    }
                }
                clicked = true;
            } else
            {
                clicked = false;
            }
        }
    }
};

//A test function for the button
static void button_pressed(Entity* entity)
{
    print_debug("Button pressed!");
}

//Deltatime
float delta = 0.0;

//Main window
Window main_window = Window("Window", 1024, 720);

int main(int argc, char* argv[])
{
    //Initialization stuff
    pomegranate_init();
    srand((unsigned int)time(nullptr));
    print_pass("Pomegranate Initialized");
    main_window.open();
    main_window.make_current(); // This makes the window the current rendering target
    print_pass("Window opened: " + std::string(main_window.get_title()) + " with resolution of " + std::to_string(main_window.get_width()) + "x" + std::to_string(main_window.get_height()));

//region Physics Example
    EntityGroup group = EntityGroup();
    auto camera = new Entity();
    camera->add_component<Camera>();
    camera->add_component<Transform>();
    Camera::make_current(camera);
    group.add_entity(camera);
    for(int z = -1; z <= 1; z++)
    {
        for (int i = -1; i <= 1; i++) {
            auto* physics_body = new Entity();
            physics_body->add_component<PhysicsObject>();
            physics_body->add_component<Transform>();
            physics_body->get_component<Transform>()->scale = Vec2(0.25f, 0.25f);
            physics_body->add_component<Sprite>();
            auto* s = physics_body->get_component<Sprite>();
            s->load_texture("res/pomegranate.png");

            physics_body->add_component<CollisionShape>();
            auto* c = physics_body->get_component<CollisionShape>();
            c->radius = 128.0;
            c->restitution = 0.0;

            physics_body->get_component<Transform>()->pos = Vec2(256.0f + (float)z * 32.0f, 16.0f + (float)i * 32.0f);
            group.add_entity(physics_body);
        }
    }

    EntityGroup world = EntityGroup();

    for (int i = -32; i <= 32; ++i)
    {
        auto *collision_body = new Entity();
        collision_body->add_component<CollisionShape>();
        collision_body->add_component<Transform>();
        collision_body->add_component<PhysicsObject>();
        auto *p = collision_body->get_component<PhysicsObject>();
        p->body_type = PHYSICS_BODY_TYPE_STATIC;
        collision_body->get_component<Transform>()->pos = Vec2(256.0f + 8.0f * (float) i,
                                                               256.0f + 196.0f - (float) (abs(i) * abs(i)) * 0.2f);
        collision_body->add_component<DebugCircle>();
        auto *d = collision_body->get_component<DebugCircle>();
        d->color = Color(34, 221, 255, 255);

        world.add_entity(collision_body);
    }


    group.add_system(new RigidBody());
    group.add_system(new Drag());

//endregion

//region Tilemap Example
    auto* tilemap = new Entity();
    tilemap->add_component<Tilemap>();
    tilemap->add_component<Transform>();
    auto* t = tilemap->get_component<Tilemap>();
    auto* s = new Sprite();
    s->load_texture("res/tilesheet.png");
    t->load_texture("res/tilesheet.png");
    t->set_tileset_tile_size(16,16);
    t->add_layer();
    print_pass("Added layer");
    for (int i = 1; i < 15; ++i)
    {
        t->set_tile(Vec2i(i, 10), Vec2i(8, 0), 1);
    }
    t->set_tile(Vec2i(0, 10), Vec2i(8, 1), 1);
    t->set_tile(Vec2i(15, 10), Vec2i(9, 1), 1);

    for(int i = 1; i < 7; ++i)
    {
        t->set_tile(Vec2i(0, i + 10), Vec2i(10, 1), 1);
        t->set_tile(Vec2i(15, i + 10), Vec2i(12, 1), 1);
        for (int j = 1; j < 15; ++j) {
            t->set_tile(Vec2i(j, i + 10), Vec2i(11, 1), 1);
        }
    }
    for (int i = 1; i < 15; ++i) {
        int r = rand()%10;
        if(r == 0)
        {
            t->set_tile(Vec2i(i, 9), Vec2i(11, 3), 1);
        }
        else if(r == 1)
        {
            t->set_tile(Vec2i(i, 9), Vec2i(14, 5), 1);
        }
        else if(r == 2)
        {
            t->set_tile(Vec2i(i, 9), Vec2i(15, 5), 1);
        }
        else if(r == 3)
        {
            t->set_tile(Vec2i(i, 9), Vec2i(14, 6), 1);
        }
        else if(r == 4)
        {
            t->set_tile(Vec2i(i, 9), Vec2i(15, 6), 1);
        }
        else if(r == 5)
        {
            t->set_tile(Vec2i(i, 9), Vec2i(16, 5), 1);
        }
        else if(r == 6)
        {
            t->set_tile(Vec2i(i, 9), Vec2i(16, 6), 1);
        }
    }
    t->place_multitile(Vec2i(rand() % 10 + 3, 2), Vec2i(2, 0), Vec2i(6, 7), 0);
    print_pass("Tiles done");
    tilemap->get_component<Transform>()->scale*=3.0;
    tilemap->get_component<Transform>()->pos+= Vec2(720, 0);
    world.add_entity(tilemap);

    for (int i = 0; i < Entity::entities.size(); ++i) {
        Entity::entities[i]->id = i;
    }

//endregion

//region UI Example
    EntityGroup UI = EntityGroup();
    auto* text = new Entity();
    text->add_component<UIText>();
    text->add_component<UITransform>();
    text->add_component<UIInteraction>();
    //Set the text to something
    text->get_component<UIText>()->text = "Hello UI!";
    text->get_component<UITransform>()->pos = Vec2(0.5, 0.05);
    UI.add_entity(text);


    auto* image = new Entity();
    image->add_component<UIImage>();
    image->add_component<UITransform>();
    image->add_component<UIInteraction>();
    image->get_component<UITransform>()->pos = Vec2(0.25, 0.1);
    image->get_component<UITransform>()->size = Vec2(0.1, 0.1);
    image->get_component<UIImage>()->load_texture("res/9sliced.png");
    UI.add_entity(image);

    auto* button = new Entity();
    button->add_component<UIButton>();
    button->add_component<UIImage>();
    button->add_component<UIText>();
    button->add_component<UIInteraction>();
    button->add_component<UITransform>();
    button->get_component<UITransform>()->pos = Vec2(0.75, 0.1);
    button->get_component<UITransform>()->size = Vec2(0.1, 0.1);
    button->get_component<UIText>()->text = "Click me!";
    button->get_component<UIButton>()->callback = button_pressed;
    button->get_component<UIImage>()->load_texture("res/9sliced.png");
    UI.add_entity(button);

    auto* text_field = new Entity();
    text_field->add_component<UITextField>();
    text_field->add_component<UIText>();
    text_field->add_component<UIImage>();
    text_field->add_component<UITransform>();
    text_field->get_component<UITransform>()->pos = Vec2(0.5, 0.1);
    text_field->get_component<UITransform>()->size = Vec2(0.33, 0.025);
    text_field->get_component<UIImage>()->load_texture("res/9sliced.png");
    UI.add_entity(text_field);

    auto* slider = new Entity();
    slider->add_component<UISlider>();
    slider->add_component<UITransform>();
    slider->add_component<UIInteraction>();
    slider->add_component<UIImage>();
    slider->get_component<UITransform>()->pos = Vec2(0.5, 0.2);
    slider->get_component<UITransform>()->size = Vec2(0.33, 0.025);
    slider->get_component<UIImage>()->load_texture("res/9sliced.png");
    slider->get_component<UIInteraction>()->ignore_input = true;
    slider->add_component<UIText>();
    slider->get_component<UIText>()->text = "Gravity Multiplier";

    UI.add_entity(slider);


    auto* slider_handle = new Entity();
    slider_handle->add_component<UIImage>();
    slider_handle->add_component<UITransform>();
    slider_handle->add_component<UIInteraction>();
    slider_handle->get_component<UITransform>()->pos = Vec2(0.5, 0.2);
    slider_handle->get_component<UITransform>()->size = Vec2(0.05, 0.05);
    slider->get_component<UISlider>()->slider_handle = slider_handle;
    slider_handle->get_component<UIImage>()->load_texture("res/9sliced.png");
    slider_handle->get_component<UITransform>()->z_index = 1;

    UI.add_entity(slider_handle);


    UI.add_system(new UIController());
    print_pass("Added UI");
//endregion

    //Add global systems
    System::add_global_system(new TransformLinkages());
    System::add_global_system(new Render());


    float tick_time = 0.0;
    bool is_running = true;
    SDL_Event event;

    while (is_running)
    {
        Uint64 start = SDL_GetPerformanceCounter(); //For delta time

        InputManager::update();
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT) {
                is_running = false;
            }
        }


        PhysicsObject::gravity = Vec2(0.0, 980.0)*slider->get_component<UISlider>()->value;

        //- - - - - # UPDATE # - - - - -
        if (tick_time > 0.016)
        {
            tick_time = 0.0;

            //tick systems
            System::global_system_tick();
            group.tick();
            world.tick();
            UI.tick();

            //Move camera (Not correct way to do this)
            if(InputManager::get_key(SDL_SCANCODE_LEFT))
            {
                camera->get_component<Transform>()->pos.x -= 8.0f;
            }
            if(InputManager::get_key(SDL_SCANCODE_RIGHT))
            {
                camera->get_component<Transform>()->pos.x += 8.0f;
            }
            if(InputManager::get_key(SDL_SCANCODE_UP))
            {
                camera->get_component<Transform>()->pos.y -= 8.0f;
            }
            if(InputManager::get_key(SDL_SCANCODE_DOWN))
            {
                camera->get_component<Transform>()->pos.y += 8.0f;
            }
        }

        //- - - - - # DRAW # - - - - -

        //Clear
        SDL_SetRenderDrawColor(Window::current->get_sdl_renderer(), 7, 14, 14, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(Window::current->get_sdl_renderer());
        SDL_SetRenderDrawColor(Window::current->get_sdl_renderer(), 255, 255, 255, 255);

        //Draw
        System::global_system_draw(Transform::draw_sort);
        group.draw(Transform::draw_sort);
        world.draw(Transform::draw_sort);
        UI.draw(UITransform::draw_sort);

        SDL_RenderPresent(Window::current->get_sdl_renderer()); //Present

        //Calculate delta time
        Uint64 end = SDL_GetPerformanceCounter();
        float secondsElapsed = (float)(end - start) / (float)SDL_GetPerformanceFrequency();
        delta = secondsElapsed;
        tick_time += delta;
    }

    pomegranate_quit(); //Cleanup
    return 0;
}
