#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <string>
#include <memory>

using namespace std;

struct BlockData {
	string name;
	shared_ptr<SDL_Texture> texture;
	int x;
	int y;
	int w;
	int h;
	bool permissable;
	bool front;
	float traction;

	BlockData(string n, shared_ptr<SDL_Texture> t);
	BlockData(string n, shared_ptr<SDL_Texture> t, int x1, int y1, int w1, int h1, float tr = 0, bool p = false, bool f = false);
};

bool dataCompare(BlockData* a, BlockData* b);