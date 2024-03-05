#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <string>

using namespace std;

struct BlockData {
	string name;
	SDL_Texture* texture;
	int x;
	int y;
	int w;
	int h;
	bool permissable;

	BlockData(string n, SDL_Texture* t);
	BlockData(string n, SDL_Texture* t, int x1, int y1, int w1, int h1, bool p = false);
};

bool dataCompare(BlockData* a, BlockData* b);