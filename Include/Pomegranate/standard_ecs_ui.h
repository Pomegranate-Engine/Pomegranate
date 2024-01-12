#ifndef POMEGRANATE_ENGINE_STANDARD_ECS_UI_H
#define POMEGRANATE_ENGINE_STANDARD_ECS_UI_H
#include <string>
#include <functional>
#include "ecs.h"
#include "color.h"
#include "vec_math.h"
#include "window.h"

enum HorizontalAlignment
{
    ALIGNMENT_HORIZONTAL_LEFT,
    ALIGNMENT_HORIZONTAL_CENTER,
    ALIGNMENT_HORIZONTAL_RIGHT
};

enum VerticalAlignment
{
    ALIGNMENT_VERTICAL_TOP,
    ALIGNMENT_VERTICAL_CENTER,
    ALIGNMENT_VERTICAL_BOTTOM
};

enum SliderOrientation
{
    SLIDER_ORIENTATION_HORIZONTAL,
    SLIDER_ORIENTATION_VERTICAL
};

class UIInteraction : public Component
{
public:
    bool mouse_over;
    bool mouse_down;
    bool ignore_input;
    std::function<void(Entity*)> click_callback;
    UIInteraction();
};

class UITransform : public Component
{
public:
    bool visible;
    bool screen_space;
    Vec2 pos;
    Vec2 size;
    Vec2 anchor;
    float rot;
    int z_index;
    HorizontalAlignment horizontal_alignment;
    VerticalAlignment vertical_alignment;
    UITransform();
    Vec2 get_screen_pos(int width, int height) const;
    Vec2 get_pos(int width, int height) const;
    Vec2 get_screen_size(int width, int height) const;
    Vec2 get_size(int width, int height) const;
    static Vec2 pixel_to_screen_space(Vec2 pixel_pos, int width, int height);
    static bool draw_sort(Entity* a, Entity* b);
};


class UIButton : public Component
{
public:
    bool pressed;
    std::function<void(Entity*)> callback;
    UIButton();
    void init(Entity *) override;
};

class UIImage : public Component
{
public:
    SDL_Texture* texture;
    Color color;
    UIImage();
    void load_texture(const char* path);
};
class UI9SlicedImage : public Component
{
public:
    SDL_Texture* texture;
    int left;
    int right;
    int top;
    int bottom;
    Color color;
    UI9SlicedImage();
    void load_texture(const char* path);
};

class UIText : public Component
{
public:
    TTF_Font* font;
    std::string text;
    Color color;
    UIText();
};

class UITextField : public Component
{
public:
    //Text data is stored in UIText
    int max_length;
    int edit_position{};
    UITextField();
    void init(Entity *) override;
};

class UICheckBox : public Component
{
public:
    bool checked;
    UICheckBox();
    void init(Entity *) override;
};

class UISlider : public Component
{
public:
    float value;
    float min;
    float max;
    Entity* slider_handle;
    SliderOrientation orientation;
    UISlider();
    void init(Entity *) override;
};

class UIController : public System
{
public:
    UIController();
    void tick(Entity* entity) override;
    void draw(Entity* entity) override;
    static void interaction(Entity* entity);
    static void text(Entity* entity);
    static void image(Entity* entity);
    static void text_field(Entity* entity);
    static void check_box(Entity* entity);
    static void on_text_field_click(Entity* entity);
    static void on_check_box_click(Entity* entity);
    static void on_button_click(Entity* entity);
    static void nine_sliced_image(Entity *entity);
    static void slider(Entity *entity);
    static Entity* focused;
    static bool mouse_down;
};

#endif //POMEGRANATE_ENGINE_STANDARD_ECS_UI_H
