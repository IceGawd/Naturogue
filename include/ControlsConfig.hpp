#pragma once

#include <SDL2/SDL.h>
#include <vector>

using namespace std;

struct ControlsConfig {
	vector<SDL_Scancode> up = {SDL_SCANCODE_UP, SDL_SCANCODE_W};
	vector<SDL_Scancode> down = {SDL_SCANCODE_DOWN, SDL_SCANCODE_S};
	vector<SDL_Scancode> left = {SDL_SCANCODE_LEFT, SDL_SCANCODE_A};
	vector<SDL_Scancode> right = {SDL_SCANCODE_RIGHT, SDL_SCANCODE_D};
	vector<SDL_Scancode> okay = {SDL_SCANCODE_Z, SDL_SCANCODE_RETURN, SDL_SCANCODE_SPACE};
	vector<SDL_Scancode> back = {SDL_SCANCODE_X, SDL_SCANCODE_BACKSPACE, SDL_SCANCODE_ESCAPE};
};