#include"engine.h"


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

int pomegranate_init(bool input, bool video, bool audio, bool img, bool ttf)
{
    auto* compiled = (SDL_version*)malloc(sizeof(SDL_version));
    SDL_GetVersion(compiled);
    print_log(std::to_string(compiled->major) + "." + std::to_string(compiled->minor) + "." + std::to_string(compiled->patch));
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