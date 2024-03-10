//A test function for the button
static void button_pressed(Entity* entity)
{
    print_debug("Button pressed!");
}

EntityGroup* build_scene()
{
    Component::register_component(Camera);
    Component::register_component(Transform);
    Component::register_component(UIText);

    //region Physics Example
    auto* group = new EntityGroup("PHYSICS");
    auto camera = new Entity();
    camera->add_component<Camera>();
    camera->add_component<Transform>();
    camera->add_component<LuaComponent>("res/scripts/CameraControllerComponent.lua");


    Camera::make_current(camera);
    group->add_entity(camera);
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
            group->add_entity(physics_body);
        }
    }

    auto* world = new EntityGroup("WORLD");

    auto* debug_circle = new Entity();
    debug_circle->add_component<DebugCircle>();
    debug_circle->add_component<Transform>();
    debug_circle->get_component<Transform>()->pos = Vec2(0.0f,0.0f);
    debug_circle->get_component<DebugCircle>()->color = Color(255, 0, 0, 255);
    debug_circle->get_component<DebugCircle>()->radius = 512.0f;

    /*for (int i = -32; i <= 32; ++i)
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

        world->add_entity(collision_body);
    }*/


    group->add_system(new RigidBody());
    group->add_system(new Drag());

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
    world->add_entity(tilemap);

//endregion
    print_pass("Added UI");

    auto* ui = new EntityGroup("UI");
    auto* text = new Entity();
    text->add_component<UIText>();
    text->get_component<UIText>()->text = "Hello World!";
    text->get_component<UIText>()->color = Color(255, 255, 255, 255);
    text->get_component<UITransform>()->position = Vec2(0, 0);
    //Create button
    auto* button = new Entity();
    button->add_component<UITransform>();
    button->get_component<UITransform>()->position = Vec2(0, 16);
    button->get_component<UITransform>()->size = Vec2(128, 32);
    button->add_component<UIButton>();
    button->get_component<UIButton>()->text = "Button";
    button->get_component<UIButton>()->text_color = Color(255, 255, 255, 255);
    button->get_component<UIButton>()->background_color = Color(0, 0, 0, 255);
    button->get_component<UIButton>()->callback = button_pressed;
    ui->add_entity(text);
    //Create text field
    auto* text_field = new Entity();
    text_field->add_component<UITransform>();
    text_field->get_component<UITransform>()->position = Vec2(0, 48);
    text_field->get_component<UITransform>()->size = Vec2(128, 128);
    text_field->add_component<UITextField>();
    text_field->get_component<UITextField>()->text = "Text Field";
    text_field->get_component<UITextField>()->placeholder_text = "";
    text_field->get_component<UITextField>()->multiline = true;
    text_field->get_component<UITextField>()->text_color = Color(255, 255, 255, 255);
    text_field->get_component<UITextField>()->background_color = Color(0, 0, 0, 255);
    ui->add_entity(text_field);

    Info_Text = new Entity();
    Info_Text->add_component<UITransform>();
    Info_Text->get_component<UITransform>()->position = Vec2(512, 0);
    Info_Text->get_component<UITransform>()->size = Vec2(128, 128);
    Info_Text->add_component<UIText>();
    Info_Text->get_component<UIText>()->text = "Text Field";
    ui->add_entity(Info_Text);

    //Create dropdown
    auto* dropdown = new Entity();
    dropdown->add_component<UITransform>();
    dropdown->get_component<UITransform>()->position = Vec2(0, 176);
    dropdown->get_component<UITransform>()->size = Vec2(128, 32);
    dropdown->add_component<UIDropdown>();
    dropdown->get_component<UIDropdown>()->text = "Dropdown";
    dropdown->get_component<UIDropdown>()->options = {"Option 1", "Option 2", "Option 3"};
    dropdown->get_component<UIDropdown>()->selected_option = 0;
    dropdown->get_component<UIDropdown>()->text_color = Color(255, 255, 255, 255);
    dropdown->get_component<UIDropdown>()->background_color = Color(0, 0, 0, 255);
    ui->add_entity(dropdown);

    //Create checkbox
    checkbox = new Entity();
    checkbox->add_component<UITransform>();
    checkbox->get_component<UITransform>()->position = Vec2(0, 194);
    checkbox->get_component<UITransform>()->size = Vec2(128, 32);
    checkbox->add_component<UICheckbox>();
    checkbox->get_component<UICheckbox>()->text = "Light mode?";
    checkbox->get_component<UICheckbox>()->checked = false;
    checkbox->get_component<UICheckbox>()->text_color = Color(255, 255, 255, 255);
    checkbox->get_component<UICheckbox>()->background_color = Color(0, 0, 0, 255);
    ui->add_entity(checkbox);

    auto* slider = new Entity();
    slider->add_component<UITransform>();
    slider->get_component<UITransform>()->position = Vec2(0, 224);
    slider->get_component<UITransform>()->size = Vec2(128, 32);
    slider->add_component<UISlider>();
    slider->get_component<UISlider>()->min = 0.0;
    slider->get_component<UISlider>()->max = 1.0;
    slider->get_component<UISlider>()->value = 0.5;
    slider->get_component<UISlider>()->background_color = Color(0, 0, 0, 255);
    slider->get_component<UISlider>()->slider_color = Color(255, 255, 255, 255);
    ui->add_entity(slider);

    //Add global systems
    System::add_global_system(new TransformLinkages());
    System::add_global_system(new Render());
    System::add_global_system(new UIController());
    System::add_global_system(new Spawn());
    System::add_global_system(new Destroy());
    auto* lua_system = new LuaSystem();
    lua_system->load_script("res/scripts/CameraControllerSystem.lua");
    System::add_global_system(lua_system);
    auto* camera_controller_system = new CameraControllerSystem();
    camera_controller_system->active = !use_lua_camera_controller;
    System::add_global_system(camera_controller_system);

    lua_system->active = use_lua_camera_controller;
    //ui.add_system(new UIController());

    auto* scene = new EntityGroup("SCENE");
    scene->add_group(group);
    scene->add_group(world);
    scene->add_group(ui);
    return scene;
}

void init()
{
    //Initialization stuff
    pomegranate_init();
    srand((unsigned int)time(nullptr));
    print_pass("Pomegranate Initialized");
    main_window.open();
    main_window.make_current(); // This makes the window the current rendering target
    main_window.set_icon("res/pomegranate.png");
    //Random window title

    int r = rand()%5;
    if(r == 0)
    {
        main_window.set_title("Pomegranate Engine - Pomegranates are berries!");
    }
    else if(r == 1)
    {
        main_window.set_title("Pomegranate Engine - Pomegranate trees live longer than 200 years!");
    }
    else if(r == 2)
    {
        main_window.set_title("Pomegranate Engine - Pomegranates are considered a superfruit!");
    }
    else if(r == 3)
    {
        main_window.set_title("Pomegranate Engine - Grenades are named after pomegranates!");
    }
    else if(r == 4)
    {
        main_window.set_title("Pomegranate Engine - Pomegranates on average have 613 seeds and can get up to 1400!");
    }


    print_pass("Window opened: " + std::string(main_window.get_title()) + " with resolution of " + std::to_string(main_window.get_width()) + "x" + std::to_string(main_window.get_height()));
    //Imgui
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Platform/Renderer backends
    ImGui_ImplSDL3_InitForSDLRenderer(main_window.get_sdl_window(),main_window.get_sdl_renderer());
    ImGui_ImplSDLRenderer3_Init(main_window.get_sdl_renderer());
}