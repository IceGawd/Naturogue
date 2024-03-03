#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Entity.hpp"

struct SpriteSheet {
	shared_ptr<SDL_Texture> texture;
	int xFrames;
	int yFrames;
	int frames;

	SpriteSheet();
	SpriteSheet(SDL_Texture* ptexture, int x, int y, int frames);
};