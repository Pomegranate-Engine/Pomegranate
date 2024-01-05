#ifndef ENGINE_H
#define ENGINE_H

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <iostream>
#include<string>

/*----------> DEBUGGING <----------*/

void print_pass(std::string);
void print_fail(std::string);
void print_error(std::string);
void print_warn(std::string);
void print_info(std::string);
void print_debug(std::string);
void print_notice(std::string);
void print_log(std::string);
void print_ready(std::string);
void print_assert(std::string);

#endif // ENGINE_H