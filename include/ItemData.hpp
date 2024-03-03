#pragma once

#include "Entity.hpp"

#include <string>

using namespace std;

struct ItemData {
	shared_ptr<SDL_Texture> texture;
	string name;
	int meleeDamage;
	int rangedDamage;

	ItemData(SDL_Texture* t, string n, int m, int r) : name(n), meleeDamage(m), rangedDamage(r) {
		texture = shared_ptr<SDL_Texture>(t, sdl_deleter());
	}
};