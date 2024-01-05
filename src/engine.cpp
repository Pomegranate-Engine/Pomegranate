#include"engine.h"

vec2::vec2()
{
    this->x = 0.0;
    this->y = 0.0;
}
vec2::vec2(float x, float y)
{
    this->x = x;
    this->y = y;
}
vec2 vec2::operator+(const vec2& other) const
{
    return {this->x + other.x, this->y + other.y};
}
vec2 vec2::operator-(const vec2& other) const
{
    return {this->x - other.x, this->y - other.y};
}
vec2 vec2::operator*(const vec2& other) const
{
    return {this->x * other.x, this->y * other.y};
}
vec2 vec2::operator*(const float& other) const
{
    return {this->x * other, this->y * other};
}
vec2 vec2::operator/(const vec2& other) const
{
    return {this->x / other.x, this->y / other.y};
}
vec2 vec2::operator/(const float& other) const
{
    return {this->x / other, this->y / other};
}
vec2 vec2::operator+=(const vec2& other)
{
    this->x += other.x;
    this->y += other.y;
    return *this;
}
vec2 vec2::operator-=(const vec2& other)
{
    this->x -= other.x;
    this->y -= other.y;
    return *this;
}
vec2 vec2::operator*=(const vec2& other)
{
    this->x *= other.x;
    this->y *= other.y;
    return *this;
}
vec2 vec2::operator*=(const float& other)
{
    this->x *= other;
    this->y *= other;
    return *this;
}
vec2 vec2::operator/=(const vec2& other)
{
    this->x /= other.x;
    this->y /= other.y;
    return *this;
}
vec2 vec2::operator/=(const float& other)
{
    this->x /= other;
    this->y /= other;
    return *this;
}
bool vec2::operator==(const vec2& other) const
{
    return this->x == other.x && this->y == other.y;
}
bool vec2::operator!=(const vec2& other) const
{
    return this->x != other.x || this->y != other.y;
}
float vec2::dot(const vec2& other) const
{
    return this->x * other.x + this->y * other.y;
}
float vec2::cross(const vec2& other) const
{
    return this->x * other.y - this->y * other.x;
}
float vec2::length() const
{
    return sqrt(this->x * this->x + this->y * this->y);
}
vec2 vec2::normalized() const
{
    float l = this->length();
    return {this->x / l, this->y / l};
}
vec2 vec2::rotate(float angle) const
{
    return {this->x * cos(angle) - this->y * sin(angle), this->x * sin(angle) + this->y * cos(angle)};
}
vec2 vec2::lerp(const vec2& other, float t) const
{
    return {this->x + (other.x - this->x) * t, this->y + (other.y - this->y) * t};
}
vec2 vec2::slerp(const vec2& other, float t) const
{
    float theta = acos(this->dot(other) / (this->length() * other.length()));
    return (this->rotate(theta * t) * (1.0f - t)) + (other * t);
}
vec2 vec2::direction_to(const vec2& other) const
{
    return (other - *this).normalized();
}
float vec2::distance_to(const vec2& other) const
{
    return (other - *this).length();
}
float vec2::angle_to(const vec2& other) const
{
    return atan2(other.y - this->y, other.x - this->x);
}


Window::Window(const char* title, int width, int height)
{
    this->title = title;
    this->width = width;
    this->height = height;

    this->window = nullptr;
    this->renderer = nullptr;
}

Window::~Window()
{
    this->close();
}

unsigned int Window::get_width() const
{
    return this->width;
}

unsigned int Window::get_height() const
{
    return this->height;
}

SDL_Window* Window::get_sdl_window() const
{
    return this->window;
}
SDL_Renderer* Window::get_sdl_renderer() const
{
    return this->renderer;
}

int Window::open()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "Failed to initialize the SDL2 library\n";
        return -1;
    }

    this->window = SDL_CreateWindow(this->title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, this->width, this->height, 0);

    if (!this->window)
    {
        std::cout << "Failed to create window\n" << std::endl;
        return -1;
    }

    this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED);

    if (!this->renderer)
    {
        SDL_DestroyWindow(this->window);
        SDL_Quit();
        std::cout << "Failed to create renderer\n" << std::endl;
        return -1;
    }
    return 0;
}

void Window::close()
{
    SDL_DestroyWindow(this->window);
    SDL_DestroyRenderer(this->renderer);
}

const char *Window::get_title() const
{
    return this->title;
}
void Window::make_current()
{
    Window::current = this;
}
Window* Window::current = nullptr;


// Define macros for text formatting
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define ITALIC  "\033[3m"
#define UNDERLINE "\033[4m"

// Define macros for text colors
#define BLACK   "\033[30m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"


// Define macros for background colors
#define BG_BLACK   "\033[40m"
#define BG_RED     "\033[41m"
#define BG_GREEN   "\033[42m"
#define BG_YELLOW  "\033[43m"
#define BG_BLUE    "\033[44m"
#define BG_MAGENTA "\033[45m"
#define BG_CYAN    "\033[46m"
#define BG_WHITE   "\033[47m"

#define BG_BRIGHT_BLACK   "\033[100m"
#define BG_BRIGHT_RED     "\033[101m"
#define BG_BRIGHT_GREEN   "\033[102m"
#define BG_BRIGHT_YELLOW  "\033[103m"
#define BG_BRIGHT_BLUE    "\033[104m"
#define BG_BRIGHT_MAGENTA "\033[105m"
#define BG_BRIGHT_CYAN    "\033[106m"
#define BG_BRIGHT_WHITE   "\033[107m"

void print_pass(std::string text)
{
    std::cout << WHITE << BG_GREEN << " PASS " << RESET << " " << text << std::endl;
}
void print_fail(std::string text)
{
    std::cout << WHITE << BG_BRIGHT_RED << " FAIL " << RESET << " " << text << std::endl;
}
void print_error(std::string text)
{
    std::cout << WHITE << BG_RED << " ERROR " << RESET << " " << text << std::endl;
}
void print_warn(std::string text)
{
    std::cout << WHITE << BG_BRIGHT_YELLOW << " WARN " << RESET << " " << text << std::endl;
}
void print_info(std::string text)
{
    std::cout << WHITE << BG_BRIGHT_BLUE << " INFO " << RESET << " " << text << std::endl;
}
void print_debug(std::string text)
{
    std::cout << WHITE << BG_BRIGHT_BLACK << " DEBUG " << RESET << " " << text << std::endl;
}
void print_notice(std::string text)
{
    std::cout << WHITE << BG_CYAN << " NOTICE " << RESET << " " << text << std::endl;
}
void print_log(std::string text)
{
    std::cout << WHITE << BG_BRIGHT_BLACK << " LOG " << RESET << " " << text << std::endl;
}
void print_ready(std::string text)
{
    std::cout << WHITE << BG_MAGENTA << " READY " << RESET << " " << text << std::endl;
}
void print_assert(std::string text)
{
    std::cout << WHITE << BG_BRIGHT_BLACK << " ASSERT " << RESET << " " << text << std::endl;
}

Entity::Entity()
{
    Entity::entities.push_back(this);
    this->id = Entity::entity_count++;
    print_log("Added entity: " + std::to_string(this->id));
}

uint64_t Entity::get_id() const {
    return this->id;
}

Entity::~Entity()
{
    /*
    for (int i = 0; i < Entity::entities.size(); i++)
    {
        if(Entity::entities[i] == this)
        {
            Entity::entities.erase(Entity::entities.begin() + i);
        }
    }
    */
}


void Entity::add_component(Component* component)
{
    this->components.push_back(component);
}

void Entity::remove_component(Component* component)
{
    //this->components.erase(std::remove(this->components.begin(), this->components.end(), component), this->components.end());
}

uint64_t Entity::entity_count = 0;


System::System() = default;

System::~System() = default;

void System::init(Entity*) {}
void System::tick(Entity*) {}
void System::draw(Entity*) {}

EntityGroup::EntityGroup()
{
    this->entities = std::vector<Entity*>();
    this->systems = std::vector<System*>();
    this->child_groups = std::vector<EntityGroup>();
}

EntityGroup::~EntityGroup()
{

}

void EntityGroup::add_entity(Entity* entity)
{
    entity->add_to_group(this);
    this->entities.push_back(entity);
}

void EntityGroup::remove_entity(Entity* entity)
{
    for (int i = 0; i < entities.size(); ++i)
    {
        if (entities[i]->get_id() == entity->get_id())
        {
            entities[i]->remove_from_group(this);
            entities.erase(entities.begin() + i);
            return;
        }
    }
    for(int i = 0; i < this->child_groups.size(); ++i)
    {
        this->child_groups[i].remove_entity(entity);
    }
}

void EntityGroup::add_system(System * system)
{
    this->systems.push_back(system);
}

void EntityGroup::remove_system(System * system)
{

}

void EntityGroup::add_group(EntityGroup entityGroup)
{
    this->child_groups.push_back(entityGroup);
}

void EntityGroup::remove_group(EntityGroup entityGroup)
{

}

void EntityGroup::tick()
{
    for(int i = 0; i < this->entities.size(); ++i)
    {
        for(auto & system : this->systems)
        {
            system->tick(this->entities[i]);
        }
    }
#pragma omp parallel for
    for(int i = 0; i < this->child_groups.size(); ++i)
    {
        this->child_groups[i].tick();
    }
#pragma omp barrier
}

void EntityGroup::draw()
{
    for(auto & entity : this->entities)
    {
        for(auto & system : this->systems)
        {
            system->draw(entity);
        }
    }
    for(auto & group : this->child_groups)
    {
        group.draw();
    }
}

std::vector<Entity*> Entity::entities = std::vector<Entity*>();

void Entity::add_to_group(EntityGroup * group)
{
    this->parents.push_back(group);
}

void Entity::remove_from_group(EntityGroup * group)
{
    for (int i = 0; i < this->parents.size(); ++i)
    {
        if (this->parents[i] == group)
        {
            this->parents.erase(this->parents.begin() + i);
            return;
        }
    }
}

std::vector<EntityGroup*> Entity::get_parent_groups()
{
    return this->parents;
}

InputManager::InputManager() {

}

InputManager::~InputManager() {

}

void InputManager::update() {
    InputManager::mouse_delta = vec2();
    InputManager::mouse_moved = false;
    InputManager::mouse_scrolled = false;
    InputManager::mouse_scroll = vec2();

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_KEYDOWN:
                InputManager::keys[event.key.keysym.scancode] = true;
                break;
            case SDL_KEYUP:
                InputManager::keys[event.key.keysym.scancode] = false;
                break;
            case SDL_MOUSEBUTTONDOWN:
                InputManager::mouse_buttons[event.button.button] = true;
                break;
            case SDL_MOUSEBUTTONUP:
                InputManager::mouse_buttons[event.button.button] = false;
                break;
            case SDL_MOUSEMOTION:
                InputManager::mouse_position = vec2(event.motion.x, event.motion.y);
                InputManager::mouse_delta = vec2(event.motion.xrel, event.motion.yrel);
                InputManager::mouse_moved = true;
                break;
            case SDL_MOUSEWHEEL:
                InputManager::mouse_scroll = vec2(event.wheel.x, event.wheel.y);
                InputManager::mouse_scrolled = true;
                break;
            default:
                break;
        }
    }
}

bool InputManager::get_key(int key_code) {
    return InputManager::keys[key_code];
}

bool InputManager::get_mouse_button(int mouse_code) {
    return InputManager::mouse_buttons[mouse_code];
}

vec2 InputManager::get_mouse_position() {
    return InputManager::mouse_position;
}

vec2 InputManager::get_mouse_delta() {
    return InputManager::mouse_delta;
}

bool InputManager::get_mouse_moved() {
    return InputManager::mouse_moved;
}

bool InputManager::get_mouse_scrolled() {
    return InputManager::mouse_scrolled;
}

vec2 InputManager::get_mouse_scroll() {
    return InputManager::mouse_scroll;
}

void InputManager::init()
{
    InputManager::keys = new bool[SDL_NUM_SCANCODES];
    InputManager::mouse_buttons = new bool[8];
    for (int i = 0; i < SDL_NUM_SCANCODES; ++i) {
        InputManager::keys[i] = false;
    }
    for (int i = 0; i < 8; ++i) {
        InputManager::mouse_buttons[i] = false;
    }
    InputManager::mouse_position = vec2();
    InputManager::mouse_delta = vec2();
    InputManager::mouse_moved = false;
    InputManager::mouse_scrolled = false;
    InputManager::mouse_scroll = vec2();
}

bool* InputManager::keys;
bool* InputManager::mouse_buttons;
vec2 InputManager::mouse_position;
vec2 InputManager::mouse_delta;
bool InputManager::mouse_moved;
bool InputManager::mouse_scrolled;
vec2 InputManager::mouse_scroll;