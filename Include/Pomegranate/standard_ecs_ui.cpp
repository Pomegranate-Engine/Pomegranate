#include "standard_ecs_ui.h"

void UIController::tick(Entity *entity)
{

}
void UIController::pre_draw()
{
    //Begin full screen window
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::Begin("FULL SCREEN", nullptr,
                 ImGuiWindowFlags_NoDecoration |
    ImGuiWindowFlags_NoMove |
    ImGuiWindowFlags_NoResize |
    ImGuiWindowFlags_NoSavedSettings |
    ImGuiWindowFlags_NoFocusOnAppearing |
    ImGuiWindowFlags_NoNav |
    ImGuiWindowFlags_NoBringToFrontOnFocus |
    ImGuiWindowFlags_NoBackground);

    //Remove window margin
    ImGui::SetWindowPos(ImVec2(0, 0));
    ImGui::SetWindowSize(ImVec2(Window::current->get_width(), Window::current->get_height()));
}
void UIController::draw(Entity *entity)
{
    if(entity->has_component<UITransform>())
    {
        auto transform = entity->get_component<UITransform>();
        ImGui::SetCursorPos(ImVec2(transform->position.x, transform->position.y));
        if (entity->has_component<UIText>())
        {
            auto text = entity->get_component<UIText>();
            ImGui::TextColored(ImVec4((float) text->color.r / 255.0f, (float) text->color.g / 255.0f,
                                      (float) text->color.b / 255.0f, (float) text->color.a / 255.0f),
                               text->text.c_str());
        }
        if (entity->has_component<UIButton>())
        {
            auto button = entity->get_component<UIButton>();
            if (ImGui::Button(button->text.c_str(), ImVec2(transform->size.x, transform->size.y)))
            {
                if(button->callback != nullptr)
                    button->callback(entity);
            }
        }
        if (entity->has_component<UITextField>())
        {
            auto text_field = entity->get_component<UITextField>();
            //Set size
            ImGui::SetNextItemWidth(transform->size.x);
            ImGui::InputText("##label", &text_field->text, ImGuiInputTextFlags_EnterReturnsTrue);
        }
        if (entity->has_component<UIDropdown>())
        {
            auto dropdown = entity->get_component<UIDropdown>();
            //Set size
            ImGui::SetNextItemWidth(transform->size.x);
            if(ImGui::BeginCombo("##label", dropdown->options[dropdown->selected_option].c_str()))
            {
                for (int n = 0; n < dropdown->options.size(); n++)
                {
                    bool is_selected = (dropdown->selected_option == n);
                    if (ImGui::Selectable(dropdown->options[n].c_str(), is_selected))
                    {
                        dropdown->selected_option = n;
                    }
                    if (is_selected)
                    {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }
        }
        if (entity->has_component<UICheckbox>())
        {
            auto checkbox = entity->get_component<UICheckbox>();
            //Set size
            ImGui::SetNextItemWidth(transform->size.x);
            ImGui::Checkbox(checkbox->text.c_str(), &checkbox->checked);
        }
    }
}

void UIController::post_draw()
{
    ImGui::End();
    ImGui::PopStyleVar();
}

UIController::UIController()
= default;

UIText::UIText()
{
    this->text = "";
    this->color = Color(255, 255, 255, 255);
}

void UIText::init(Entity *entity)
{
    entity->require_component<UITransform>();
}

UIButton::UIButton()
{
    this->text = "";
    this->text_color = Color(255, 255, 255, 255);
    this->background_color = Color(0, 0, 0, 255);
    this->callback = nullptr;
}

void UIButton::init(Entity *entity)
{
    entity->require_component<UITransform>();
}

UITransform::UITransform()
{
    this->position = Vec2(0, 0);
    this->size = Vec2(64, 64);
}

UITextField::UITextField()
{
    this->text = "";
    this->placeholder_text = "Enter text here";
    this->text_color = Color(255, 255, 255, 255);
    this->background_color = Color(0, 0, 0, 255);
}

void UITextField::init(Entity *entity)
{
    entity->require_component<UITransform>();
}

UIDropdown::UIDropdown()
{
    this->text = "";
    this->options = std::vector<std::string>();
    this->selected_option = 0;
    this->text_color = Color(255, 255, 255, 255);
    this->background_color = Color(0, 0, 0, 255);
}

void UIDropdown::init(Entity *entity)
{
    entity->require_component<UITransform>();
}

UICheckbox::UICheckbox()
{
    this->text = "";
    this->checked = false;
    this->text_color = Color(255, 255, 255, 255);
    this->background_color = Color(0, 0, 0, 255);
}

void UICheckbox::init(Entity *entity)
{
    entity->require_component<UITransform>();
}
