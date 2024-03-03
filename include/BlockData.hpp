#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <string>

using namespace std;

struct BlockData {
	string name;
	SDL_Texture* texture;
};