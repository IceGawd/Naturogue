#include "ItemData.hpp"

ItemData::ItemData(shared_ptr<SDL_Texture> t, int w, int h, int af, int at, int sw, int sh, string n, double am, double sa, int m, int r, int st, bool ts, bool s, int tv, float f, float k, vector<WEAPON_PROPERTIES> pr) {
	constructorBase(t, w, h, af, at, sw, sh, n, am, sa, m, r, st, ts, s, tv, f, k, pr);
	points = square(0, 0, w, h);
}

ItemData::ItemData(shared_ptr<SDL_Texture> t, int w, int h, int af, int at, int sw, int sh, string n, double am, double sa, int m, int r, int st, bool ts, bool s, int tv, float f, float k, vector<WEAPON_PROPERTIES> pr, int x, int y, int hw, int hh) {
	constructorBase(t, w, h, af, at, sw, sh, n, am, sa, m, r, st, ts, s, tv, f, k, pr);
	points = square(x, y, hw, hh);
}

ItemData::ItemData(shared_ptr<SDL_Texture> t, int w, int h, int af, int at, int sw, int sh, string n, double am, double sa, int m, int r, int st, bool ts, bool s, int tv, float f, float k, vector<WEAPON_PROPERTIES> pr, vector<Vector2f> p) {
	constructorBase(t, w, h, af, at, sw, sh, n, am, sa, m, r, st, ts, s, tv, f, k, pr);
	for (Vector2f v2f : p) {
		points.push_back(Vector2f(v2f.x * sw / w, v2f.y * sh / h));
	}
}

void ItemData::constructorBase(shared_ptr<SDL_Texture> t, int w, int h, int af, int at, int sw, int sh, string n, double am, double sa, int m, int r, int st, bool ts, bool s, int tv, float f, float k, vector<WEAPON_PROPERTIES> pr) {
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

	throwVelocity = tv;
	friction = f;
	knockback = k;

	properties = pr;
}

bool ItemData::hasProperty(WEAPON_PROPERTIES wp) {
	for (WEAPON_PROPERTIES wp2 : properties) {
		if (wp == wp2) {
			return true;
		}
	}
	return false;
}