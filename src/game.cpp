void tick(EntityGroup* scene)
{
    Info_Text->get_component<UIText>()->text = "Info\nFPS: " + std::to_string(1.0/delta_time) + "\nPhysics Bodies: " + std::to_string(PhysicsObject::objects.size());
    //tick systems
    System::global_system_tick();
    scene->tick();
}

void draw(EntityGroup* scene)
{
    //Draw
    System::global_system_draw(Transform::draw_sort);
    scene->draw(Transform::draw_sort);
}