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
	bool throwSpin;
	bool stab;

	ItemData(shared_ptr<SDL_Texture> t, int w, int h, int af, int at, int sw, int sh, string n, double am, double sa, int m, int r, int st, bool ts, bool s) {
		texture = t;
		width = w;
		height = h;
		animationFrame = af;
		animationType = at;
		show_width = sw;
		show_height = sh;

		name = n;
		swingAngle = sa;
		angleMod = am;

		meleeDamage = m;
		rangedDamage = r;
		swingTime = st;

		throwSpin = ts;
		stab = s;
	}
};