void tick(EntityGroup* scene)
{
    //tick systems
    scene->tick();
}

void draw(EntityGroup* scene)
{
    //Draw
    scene->draw(Transform::draw_sort);
}