#include "ItemData.hpp"

ItemData::ItemData(shared_ptr<SDL_Texture> t, int w, int h, int af, int at, int sw, int sh, string n, double am, double sa, int m, int r, int st, bool ts, bool s) {
	constructorBase(t, w, h, af, at, sw, sh, n, am, sa, m, r, st, ts, s);
	points = {
		{0, 0}, 
		{(float) sw, 0}, 
		{(float) sw, (float) sh}, 
		{0, (float) sh}, 
	};
}

ItemData::ItemData(shared_ptr<SDL_Texture> t, int w, int h, int af, int at, int sw, int sh, string n, double am, double sa, int m, int r, int st, bool ts, bool s, int x, int y, int hw, int hh) {
	constructorBase(t, w, h, af, at, sw, sh, n, am, sa, m, r, st, ts, s);
	points = {
		{(float) x, (float) y}, 
		{(float) (x + hw), (float) y}, 
		{(float) (x + hw), (float) (y + hh)}, 
		{(float) x, (float) (y + hh)}, 
	};
}

ItemData::ItemData(shared_ptr<SDL_Texture> t, int w, int h, int af, int at, int sw, int sh, string n, double am, double sa, int m, int r, int st, bool ts, bool s, vector<Vector2f> p) {
	constructorBase(t, w, h, af, at, sw, sh, n, am, sa, m, r, st, ts, s);
	points = p;
}

void ItemData::constructorBase(shared_ptr<SDL_Texture> t, int w, int h, int af, int at, int sw, int sh, string n, double am, double sa, int m, int r, int st, bool ts, bool s) {
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