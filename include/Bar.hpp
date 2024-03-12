#pragma once

#include "GameObject.hpp"

struct Bar : public GameObject {
	static constexpr float WAVEDELTA = M_PI / 18.0;

	float value;
	float maxValue;
	int thickness;
	SDL_Surface* surf;

	float waveMod = 0;

	Bar(RenderWindow* window, int w, int h, int t, float mv);
	virtual bool draw(RenderWindow* window, World* world, vector<GameObject*>& entities);
};
