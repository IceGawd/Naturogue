#pragma once

#include "Item.hpp"
#include "GameObject.hpp"

struct Weapon : public GameObject {
	Item* item;
	bool melee;
	float startAngle;
	int framesAlive = 0;
	float charge;
	int damage;

	bool comeBack = false;

	vector<Vector2f> points;

	~Weapon();
	Weapon(Item* i, bool m, float a, float c);
	void calculatePoints();
	virtual bool draw(RenderWindow* window, World* world, vector<GameObject*>& entities);
};
