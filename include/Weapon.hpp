#pragma once

#include "Item.hpp"
#include "GameObject.hpp"

struct Weapon : public GameObject {
	Item* item;
	bool melee;
	float startAngle;
	int framesAlive = 0;
	int charge;

	vector<Vector2f> points;

	Weapon(Item* i, bool m, float a, int c);
	void calculatePoints();
	virtual bool draw(RenderWindow* window, World* world, vector<GameObject*>& entities);
};
