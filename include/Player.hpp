#pragma once

#include "GameObject.hpp"
#include "Input.hpp"
#include "World.hpp"
#include "Slot.hpp"
#include "Weapon.hpp"
#include "Bar.hpp"

#include <math.h>

class Player : public GameObject {
public:
	int speed = 6;
	int slots = 3;
	int selectedSlot = 0;
	float traction = 0.7;
	float chargeTraction = 0.8; // IS ADDITIONAL
	float charge = 0;
	float attackAngle = 0;

	bool swing = false;
	bool yeet = false;

	bool mousedown = false;

	bool animation = false;
	
	vector<Slot> items;

	Bar* healthBar;
	Bar* chargeBar;

	Input input;
	vector<Weapon*> beingUsed;

	Player(RenderWindow* window, shared_ptr<SDL_Texture>& slotTexture, shared_ptr<SDL_Texture>& selectedSlotTexture);
	void select(int num);
	bool giveItem(Item* item);
	virtual bool draw(RenderWindow* window, World* world, vector<GameObject*>& entities);
};