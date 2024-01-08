#include "standard_ecs_ui.h"

UITransform::UITransform()
{
    this->visible = true;
    this->pos = Vec2(0.5, 0.5);
    this->size = Vec2(0.1, 0.1);
    this->anchor = Vec2(0.5, 0.5);
    this->rot = 0.0;
    this->screen_space = true;
    this->z_index = 0;
    this->horizontal_alignment = ALIGNMENT_HORIZONTAL_CENTER;
    this->vertical_alignment = ALIGNMENT_VERTICAL_CENTER;
}

Vec2 UITransform::get_screen_pos(int width, int height) const
{
    return {this->pos.x * (float)width, this->pos.y * (float)height};
}

Vec2 UITransform::get_pos(int width, int height) const
{
    if(screen_space)
    {
        return get_screen_pos(width, height);
    }
    else
    {
        return this->pos;
    }
}

Vec2 UITransform::get_screen_size(int width, int height) const {
    float aspect_ratio = (float)width / (float)height;

    float adjusted_size_x = this->size.x * (float)width;
    float adjusted_size_y = this->size.y * (float)height;

    float element_aspect_ratio = this->size.x / this->size.y;

    if (element_aspect_ratio > aspect_ratio) {
        adjusted_size_x = adjusted_size_y * element_aspect_ratio;
    } else {
        adjusted_size_y = adjusted_size_x / element_aspect_ratio;
    }

    return { adjusted_size_x, adjusted_size_y };
}



Vec2 UITransform::get_size(int width, int height) const {
    if(screen_space)
    {
        return get_screen_size(width, height);
    }
    else
    {
        return this->size;
    }
}

bool UITransform::draw_sort(Entity *a, Entity *b) {
    if(a->has_component<UITransform>() && b->has_component<UITransform>())
    {
        return a->get_component<UITransform>()->z_index < b->get_component<UITransform>()->z_index;
    }
    else
    {
        return false;
    }
}

Vec2 UITransform::pixel_to_screen_space(Vec2 pixel_pos, int width, int height)
{
    return {pixel_pos.x / (float)width, pixel_pos.y / (float)height};
}

UIText::UIText()
{
    this->font = TTF_OpenFont("res/sans.ttf", 16);
    this->text = "text";
    this->color = Color(255, 255, 255, 255);
}

void UIController::tick(Entity *entity)
{

}

void UIController::draw(Entity *entity)
{
    if(entity->has_component<UIInteraction>())
    {
        UIController::interaction(entity);
    }
    if(entity->has_component<UITextField>())
    {
        UIController::text_field(entity);
    }
    if(entity->has_component<UIImage>())
    {
        UIController::image(entity);
    }
    if(entity->has_component<UI9SlicedImage>())
    {
        UIController::nine_sliced_image(entity);
    }
    if(entity->has_component<UIText>())
    {
        UIController::text(entity);
    }
    if(entity->has_component<UICheckBox>())
    {
        UIController::check_box(entity);
    }
    if(entity->has_component<UISlider>())
    {
        UIController::slider(entity);
    }
}

void UIController::text(Entity*e)
{
    auto text = e->get_component<UIText>();
    auto trans = e->get_component<UITransform>();

    SDL_Color textColor = {(Uint8)text->color.r,(Uint8)text->color.g,(Uint8)text->color.b,(Uint8)text->color.a}; // White color

    // Create an SDL surface from the text
    SDL_Surface* surface = TTF_RenderText_Blended(text->font, text->text.c_str(), textColor);
    if (!surface) {
        // Handle error
        return;
    }

    // Create an SDL texture from the surface
    SDL_Texture* tex = SDL_CreateTextureFromSurface(Window::current->get_sdl_renderer(), surface);
    if (!tex) {
        // Handle error
        SDL_DestroySurface(surface);
        return;
    }

    // Set the position and dimensions of the text
    Vec2 pos = trans->get_pos((int)Window::current->get_width(), (int)Window::current->get_height());
    SDL_FRect destinationRect = {pos.x, pos.y, (float)surface->w, (float)surface->h};

    if(trans->horizontal_alignment == ALIGNMENT_HORIZONTAL_CENTER)
    {
        destinationRect.x -= destinationRect.w/2;
    }
    else if(trans->horizontal_alignment == ALIGNMENT_HORIZONTAL_RIGHT)
    {
        destinationRect.x -= destinationRect.w;
    }

    if(trans->vertical_alignment == ALIGNMENT_VERTICAL_CENTER)
    {
        destinationRect.y -= destinationRect.h/2;
    }
    else if(trans->vertical_alignment == ALIGNMENT_VERTICAL_BOTTOM)
    {
        destinationRect.y -= destinationRect.h;
    }

    // Render the text to the screen
    SDL_RenderTexture(Window::current->get_sdl_renderer(),tex, nullptr,&destinationRect);

    // Clean up
    SDL_DestroyTexture(tex);
    SDL_DestroySurface(surface);
}

UIController::UIController()
= default;

void UIController::image(Entity *entity)
{
    auto image = entity->get_component<UIImage>();
    auto trans = entity->get_component<UITransform>();

    SDL_Color textColor = {(Uint8)image->color.r,(Uint8)image->color.g,(Uint8)image->color.b,(Uint8)image->color.a}; // White color

    // Set the position and dimensions of the text

    Vec2 pos = trans->get_pos((int)Window::current->get_width(), (int)Window::current->get_height());
    Vec2 size = trans->get_size((int)Window::current->get_width(), (int)Window::current->get_height());
    SDL_FRect destinationRect = {pos.x, pos.y, size.x, size.y};

    if(trans->horizontal_alignment == ALIGNMENT_HORIZONTAL_CENTER)
    {
        destinationRect.x -= destinationRect.w/2;
    }
    else if(trans->horizontal_alignment == ALIGNMENT_HORIZONTAL_RIGHT)
    {
        destinationRect.x -= destinationRect.w;
    }

    if(trans->vertical_alignment == ALIGNMENT_VERTICAL_CENTER)
    {
        destinationRect.y -= destinationRect.h/2;
    }
    else if(trans->vertical_alignment == ALIGNMENT_VERTICAL_BOTTOM)
    {
        destinationRect.y -= destinationRect.h;
    }

    // Render the text to the screen
    SDL_SetRenderDrawColor(Window::current->get_sdl_renderer(), image->color.r, image->color.g, image->color.b, image->color.a);
    SDL_RenderTexture(Window::current->get_sdl_renderer(),image->texture, nullptr,&destinationRect);
}

void UIController::nine_sliced_image(Entity *entity)
{
    auto image = entity->get_component<UI9SlicedImage>();
    auto trans = entity->get_component<UITransform>();


    // Set the position and dimensions of the text

    Vec2 pos = trans->get_pos((int)Window::current->get_width(), (int)Window::current->get_height());
    Vec2 size = trans->get_size((int)Window::current->get_width(), (int)Window::current->get_height());
    SDL_FRect destinationRect = {pos.x, pos.y, size.x, size.y};

    if(trans->horizontal_alignment == ALIGNMENT_HORIZONTAL_CENTER)
    {
        destinationRect.x -= destinationRect.w/2;
    }
    else if(trans->horizontal_alignment == ALIGNMENT_HORIZONTAL_RIGHT)
    {
        destinationRect.x -= destinationRect.w;
    }

    if(trans->vertical_alignment == ALIGNMENT_VERTICAL_CENTER)
    {
        destinationRect.y -= destinationRect.h/2;
    }
    else if(trans->vertical_alignment == ALIGNMENT_VERTICAL_BOTTOM)
    {
        destinationRect.y -= destinationRect.h;
    }

    // Render the text to the screen
    SDL_SetRenderDrawColor(Window::current->get_sdl_renderer(), image->color.r, image->color.g, image->color.b, image->color.a);
    SDL_SetRenderDrawBlendMode(Window::current->get_sdl_renderer(), SDL_BLENDMODE_MUL);
    SDL_RenderTexture(Window::current->get_sdl_renderer(),image->texture, nullptr,&destinationRect);
}

void UIController::interaction(Entity *entity)
{
    auto* t = entity->get_component<UITransform>();
    auto* i = entity->get_component<UIInteraction>();
    Vec2 position = t->get_pos((int)Window::current->get_width(), (int)Window::current->get_height());
    Vec2 size = t->get_size((int)Window::current->get_width(), (int)Window::current->get_height());
    Vec2 mousepos = InputManager::get_mouse_position();

    SDL_FRect buttonRect = {position.x, position.y, size.x, size.y};

    if(t->horizontal_alignment == ALIGNMENT_HORIZONTAL_CENTER)
    {
        buttonRect.x -= buttonRect.w/2;
    }
    else if(t->horizontal_alignment == ALIGNMENT_HORIZONTAL_RIGHT)
    {
        buttonRect.x -= buttonRect.w;
    }

    if(t->vertical_alignment == ALIGNMENT_VERTICAL_CENTER)
    {
        buttonRect.y -= buttonRect.h/2;
    }
    else if(t->vertical_alignment == ALIGNMENT_VERTICAL_BOTTOM)
    {
        buttonRect.y -= buttonRect.h;
    }

    //Check if mouse is inside button
    if(SDL_PointInRectFloat(new SDL_FPoint{mousepos.x,mousepos.y}, &buttonRect))
    {
        i->mouse_over = true;
        if(!i->ignore_input)
        {
            if (InputManager::get_mouse_button(SDL_BUTTON_LEFT))
            {
                i->mouse_down = true;
                if (!UIController::mouse_down) {
                    if (i->click_callback)
                        i->click_callback(entity);
                    focused = entity;
                }
                UIController::mouse_down = true;
            } else {
                i->mouse_down = false;
                UIController::mouse_down = false;
            }
        }
        else
        {
            if (InputManager::get_mouse_button(SDL_BUTTON_LEFT))
            {
                UIController::mouse_down = true;
            }
            else
            {
                UIController::mouse_down = false;
            }
        }
    }
    else
    {
        i->mouse_over = false;
        i->mouse_down = false;
    }
}

Entity* UIController::focused = nullptr;
bool UIController::mouse_down = false;

void UIController::text_field(Entity *entity)
{
    if(focused == entity)
    {
        auto *i = entity->get_component<UIInteraction>();
        auto *t = entity->get_component<UITextField>();
        auto *text = entity->get_component<UIText>();
        if (InputManager::get_text() != -1) {
            if (InputManager::get_text() == 8)
            {
                if (t->edit_position > 0)
                {
                    text->text.erase(text->text.begin() + t->edit_position - 1);
                    t->edit_position--;
                }
            }
            else if(InputManager::get_text() == 12)
            {
                focused = nullptr;
            }
            else if(InputManager::get_text() == 1)
            {
                t->edit_position--;
                if(t->edit_position < 0)
                {
                    t->edit_position = 0;
                }
            }
            else if(InputManager::get_text() == 2)
            {
                t->edit_position++;
                if(t->edit_position > text->text.length())
                {
                    t->edit_position = text->text.length();
                }
            }
            else if ((t->max_length == 0 || text->text.length() < t->max_length) && InputManager::get_text() > 0)
            {
                text->text.insert(t->edit_position, 1, (char) InputManager::get_text());
                t->edit_position++;
            }
        }
    }
}

void UIController::check_box(Entity *entity)
{

}

void UIController::on_text_field_click(Entity *entity)
{
    print_debug("click");
    auto *t = entity->get_component<UITextField>();
    auto *text = entity->get_component<UIText>();
    t->edit_position = text->text.length();
}

void UIController::on_check_box_click(Entity *entity)
{
    printf("click\n");
    auto *c = entity->get_component<UICheckBox>();
    c->checked = !c->checked;
}

void UIController::on_button_click(Entity *entity)
{
    auto *b = entity->get_component<UIButton>();
    if(b->callback)
    {
        b->callback(entity);
    }
}

void UIController::slider(Entity *entity)
{
    auto *s = entity->get_component<UISlider>();
    auto *t = entity->get_component<UITransform>();
    auto *h = s->slider_handle->get_component<UITransform>();
    auto *i = s->slider_handle->get_component<UIInteraction>();

    Vec2 position = t->get_pos((int)Window::current->get_width(), (int)Window::current->get_height());
    Vec2 size = t->get_size((int)Window::current->get_width(), (int)Window::current->get_height());
    if(focused == s->slider_handle  && !InputManager::get_mouse_button(SDL_BUTTON_LEFT))
    {
        focused = nullptr;
    }
    if(focused == s->slider_handle && InputManager::get_mouse_button(SDL_BUTTON_LEFT))
    {
        if(s->orientation == SLIDER_ORIENTATION_HORIZONTAL)
        {
            float offset = 0.0;
            if(t->horizontal_alignment == ALIGNMENT_HORIZONTAL_CENTER)
            {
                offset = size.x/2;
            }
            else if(t->horizontal_alignment == ALIGNMENT_HORIZONTAL_RIGHT)
            {
                offset = size.x;
            }
            s->value = (InputManager::get_mouse_position().x - position.x+offset) / size.x;
            if (s->value < 0.0)
            {
                s->value = 0.0;
            }
            if (s->value > 1.0)
            {
                s->value = 1.0;
            }
            if(h->screen_space)
            {
                h->pos.x = UITransform::pixel_to_screen_space(Vec2(position.x+s->value*size.x-offset,0.0),(int)Window::current->get_width(), (int)Window::current->get_height()).x;
                print_info(std::to_string(h->pos.x));
            }
            else
            {
                h->pos.x = s->value * size.x;
            }
            h->pos.y = t->pos.y;
            s->value*=s->max;
        }
        else
        {
            s->value = (InputManager::get_mouse_position().y - position.y) / size.y;
            if (s->value < 0.0)
            {
                s->value = 0.0;
            }
            if (s->value > 1.0)
            {
                s->value = 1.0;
            }
            if(h->screen_space)
            {
                h->pos.y = InputManager::get_mouse_position().y - h->get_size((int)Window::current->get_width(), (int)Window::current->get_height()).y/2;
            }
            else
            {
                h->pos.y = s->value * size.y;
            }
            h->pos.x = position.x;
        }
    }
}

UIImage::UIImage()
{
    this->texture = IMG_LoadTexture(Window::current->get_sdl_renderer(), "res/none.png");
    this->color = Color(255, 255, 255, 255);
}

void UIImage::load_texture(const char *path)
{
    this->texture = IMG_LoadTexture(Window::current->get_sdl_renderer(), path);
}

UIButton::UIButton()
{
    this->pressed = false;
}

void UIButton::init(Entity * entity)
{
    entity->require_component<UIInteraction>();
    entity->get_component<UIInteraction>()->click_callback = UIController::on_button_click;
}

UITextField::UITextField()
{
    this->max_length = 0;
    this->edit_position = 0;
}

void UITextField::init(Entity * entity)
{
    entity->require_component<UIInteraction>();
    entity->get_component<UIInteraction>()->click_callback = UIController::on_text_field_click;
}


UICheckBox::UICheckBox()
{
    this->checked = false;
}

void UICheckBox::init(Entity * entity)
{
    entity->require_component<UIInteraction>();
    entity->get_component<UIInteraction>()->click_callback = UIController::on_check_box_click;
}

UI9SlicedImage::UI9SlicedImage()
{
    this->texture = IMG_LoadTexture(Window::current->get_sdl_renderer(), "res/none.png");
    this->color = Color(255, 255, 255, 255);
    this->left = 0;
    this->right = 0;
    this->top = 0;
    this->bottom = 0;
}

void UI9SlicedImage::load_texture(const char *path)
{
    this->texture = IMG_LoadTexture(Window::current->get_sdl_renderer(), path);
}

UISlider::UISlider()
{
    this->value = 0.0;
    this->min = 0.0;
    this->max = 1.0;
    this->slider_handle = nullptr;
    this->orientation = SLIDER_ORIENTATION_HORIZONTAL;
}

void UISlider::init(Entity * entity)
{

}

UIInteraction::UIInteraction()
{
    this->mouse_over = false;
    this->mouse_down = false;
    this->click_callback = nullptr;
}
