#pragma once

#include "GameObject.hpp"
#include "Input.hpp"
#include "World.hpp"
#include "Slot.hpp"

#include <math.h>

class Player : public GameObject {
public:
	int speed;
	int slots = 3;
	int selectedSlot = 1;
	float traction;
	
	vector<Slot> items;

	Input input;

	Player(RenderWindow* window);
	virtual bool draw(RenderWindow* window, World* world, vector<GameObject*>& entities);
};