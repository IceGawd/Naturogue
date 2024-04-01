#pragma once

#include "GameObject.hpp"

struct Bar : public GameObject {
	constexpr static float WAVEDELTA = M_PI / 18.0;

	float value;
	float maxValue;
	int thickness;
	SDL_Surface* surf;
	bool solidColor;

	float waveMod = 0;

	bool stationary;

	~Bar();
	Bar(RenderWindow* window, int w, int h, int t, float mv, bool s = false, bool so = false);
	virtual bool draw(RenderWindow* window, World* world, vector<GameObject*>& entities);
};
