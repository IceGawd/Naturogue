#pragma once

#include "Item.hpp"
#include "GameObject.hpp"

struct Weapon : public GameObject {
	Item* item;
	bool melee;
	float startAngle;
	int framesAlive = 0;

	vector<Vector2f> points;

	Weapon(Item* i, bool m, float a);
	void calculatePoints();
	virtual bool draw(RenderWindow* window, World* world, vector<GameObject*>& entities);
};
