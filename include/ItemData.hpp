#pragma once

#include "Entity.hpp"

#include <string>

using namespace std;

struct ItemData : public Entity {
	string name;
	int meleeDamage;
	int rangedDamage;
	int swingTime;
	double swingAngle;
	double angleMod;

	ItemData(shared_ptr<SDL_Texture> t, int w, int h, int af, int at, int sw, int sh, string n, int m, int r, int st, double sa, double am) : name(n), meleeDamage(m), rangedDamage(r), swingTime(st), swingAngle(sa), angleMod(am) {
		texture = t;
		width = w;
		height = h;
		animationFrame = af;
		animationType = at;
		show_width = sw;
		show_height = sh;
	}
};