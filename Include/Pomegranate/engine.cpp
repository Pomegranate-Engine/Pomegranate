#include"engine.h"

void print_pass(const std::string& text)
{
    std::cout << "\033[37m" << "\033[42m" << " PASS " << "\033[0m" << " " << text << std::endl;
}
void print_fail(const std::string& text)
{
    std::cout << "\033[37m" << "\033[101m" << " FAIL " << "\033[0m" << " " << text << std::endl;
}
void print_error(const std::string& text)
{
    std::cout << "\033[37m" << "\033[41m" << " ERROR " << "\033[0m" << " " << text << std::endl;
}
void print_warn(const std::string& text)
{
    std::cout << "\033[37m" << "\033[103m" << " WARN " << "\033[0m" << " " << text << std::endl;
}
void print_info(const std::string& text)
{
    std::cout << "\033[37m" << "\033[104m" << " INFO " << "\033[0m" << " " << text << std::endl;
}
void print_debug(const std::string& text)
{
    std::cout << "\033[37m" << "\033[100m" << " DEBUG " << "\033[0m" << " " << text << std::endl;
}
void print_notice(const std::string& text)
{
    std::cout << "\033[37m" << "\033[46m" << " NOTICE " << "\033[0m" << " " << text << std::endl;
}
void print_log(const std::string& text)
{
    std::cout << "\033[37m" << "\033[100m" << " LOG " << "\033[0m" << " " << text << std::endl;
}
void print_ready(const std::string& text)
{
    std::cout << "\033[37m" << "\033[45m" << " READY " << "\033[0m" << " " << text << std::endl;
}
void print_assert(const std::string& text)
{
    std::cout << "\033[37m" << "\033[100m" << " ASSERT " << "\033[0m" << " " << text << std::endl;
}

int pomegranate_init(bool input, bool video, bool audio, bool img, bool ttf)
{
    auto* compiled = (SDL_version*)malloc(sizeof(SDL_version));
    SDL_GetVersion(compiled);
    print_info(std::to_string(compiled->major) + "." + std::to_string(compiled->minor) + "." + std::to_string(compiled->patch));
    if(video)
    {
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            print_error("Failed to initialize the SDL2 library");
            return -1;
        }
    }
    if(audio)
    {
        if (SDL_Init(SDL_INIT_AUDIO) < 0)
        {
            print_error("Failed to initialize the SDL2 audio");
            return -1;
        }
        if(Mix_Init(MIX_INIT_WAVPACK))
        {
            print_error("Failed to initialize the SDL2 mixer");
            return -1;
        }
        if(Mix_OpenAudio( 0, nullptr))
        {
            print_error("Failed to initialize the SDL2 mixer");
            return -1;
        }
    }
    if(input)
    {
        InputManager::init();
    }
    if(img)
    {
        int imgFlags = IMG_INIT_PNG;
        if( !( IMG_Init( imgFlags ) & imgFlags ) )
        {
            print_error("SDL_image could not initialize! SDL_image Error: " + std::string(IMG_GetError()));
        }
    }
    if(ttf)
    {
        if(TTF_Init() == -1)
        {
            print_error("SDL_ttf could not initialize! SDL_ttf Error: " + std::string(TTF_GetError()));
        }
    }
    return 0;
}

void pomegranate_quit()
{
    SDL_Quit();
    Mix_Quit();
    IMG_Quit();
    TTF_Quit();
}