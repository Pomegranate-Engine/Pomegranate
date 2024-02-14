void tick(EntityGroup* scene)
{
    start_timer();
    Info_Text->get_component<UIText>()->text = "Info\nFPS: " + std::to_string(1.0/delta_time) + "\nPhysics Bodies: " + std::to_string(PhysicsObject::objects.size());
    //tick systems
    System::global_system_tick();
    scene->tick();
    std::cout << "Tick time: " << end_timer() << "ms" << std::endl;
}

void draw(EntityGroup* scene)
{
    //Draw
    start_timer();
    System::global_system_draw(Transform::draw_sort);
    scene->draw(Transform::draw_sort);
    std::cout << "Draw time: " << end_timer() << "ms" << std::endl;
}