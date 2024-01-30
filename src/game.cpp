void tick(EntityGroup* scene)
{
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