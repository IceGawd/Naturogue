#pragma once

#include "Entity.hpp"
#include "utils.hpp"

#include <string>
#include <vector>

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
	float throwVelocity;
	float friction;
	float knockback;
	vector<WEAPON_PROPERTIES> properties;
	vector<Vector2f> points;

	ItemData(shared_ptr<SDL_Texture> t, int w, int h, int af, int at, int sw, int sh, string n, double am, double sa, int m, int r, int st, bool ts, bool s, float tv, float f, float k, vector<WEAPON_PROPERTIES> pr);
	ItemData(shared_ptr<SDL_Texture> t, int w, int h, int af, int at, int sw, int sh, string n, double am, double sa, int m, int r, int st, bool ts, bool s, float tv, float f, float k, vector<WEAPON_PROPERTIES> pr, int x, int y, int hw, int hh);
	ItemData(shared_ptr<SDL_Texture> t, int w, int h, int af, int at, int sw, int sh, string n, double am, double sa, int m, int r, int st, bool ts, bool s, float tv, float f, float k, vector<WEAPON_PROPERTIES> pr, vector<Vector2f> p);

	void constructorBase(shared_ptr<SDL_Texture> t, int w, int h, int af, int at, int sw, int sh, string n, double am, double sa, int m, int r, int st, bool ts, bool s, float tv, float f, float k, vector<WEAPON_PROPERTIES> pr);
	bool hasProperty(WEAPON_PROPERTIES wp);
};