#ifndef ENGINE_H
#define ENGINE_H

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include <iostream>
#include<string>
#include "input_manager.h"
/*----------> DEBUGGING <----------*/

void print_pass(const std::string&);
void print_fail(const std::string&);
void print_error(const std::string&);
void print_warn(const std::string&);
void print_info(const std::string&);
void print_debug(const std::string&);
void print_notice(const std::string&);
void print_log(const std::string&);
void print_ready(const std::string&);
void print_assert(const std::string&);
int pomegranate_init(bool input = true, bool video = true, bool audio = true, bool img = true, bool ttf = true);
void pomegranate_quit();

#endif // ENGINE_H