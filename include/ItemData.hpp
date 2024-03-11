#pragma once

#include "Entity.hpp"

#include <string>

using namespace std;

struct ItemData {
	shared_ptr<SDL_Texture> texture;
	string name;
	int meleeDamage;
	int rangedDamage;
	int swingTime;
	float swingAngle;
	float angleMod;

	ItemData(SDL_Texture* t, string n, int m, int r, int st, float sa, float am) : name(n), meleeDamage(m), rangedDamage(r), swingTime(st), swingAngle(sa), angleMod(am) {
		texture = shared_ptr<SDL_Texture>(t, sdl_deleter());
	}
};