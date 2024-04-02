#pragma once

#include "GameObject.hpp"
#include "Input.hpp"
#include "World.hpp"
#include "Slot.hpp"
#include "Weapon.hpp"
#include "Bar.hpp"

#include <math.h>

void interact(vector<void*> objects);

class Player : public GameObject {
public:
	const static int MAXIMUM_HP = 100;
	int actualMaxHp;
	int speed = 6;
	int slots = 3;
	int selectedSlot = 0;
	float chargeTraction = 0.8; // IS ADDITIONAL
	float charge = 0;
	float attackAngle = 0;

	int HP;
	int invincibilityFrames = 0;
	int timeWithoutDamage = 0;
	bool lighter;

	bool swing = false;
	bool yeet = false;

	bool mousedown = false;

	bool animation = false;

	SDL_Texture* slotTexture;
	SDL_Texture* selectedSlotTexture;
	vector<Slot> items;

	Bar* healthBar;
	Bar* chargeBar;

	Input input;
	vector<Weapon*> beingUsed;

	Player(RenderWindow* window, SDL_Texture* slotTexture, SDL_Texture* selectedSlotTexture);
	void readyToPlay(World* world);
	void select(int num);
	float valueFromCharge();
	bool giveItem(Item* item);
	virtual bool draw(RenderWindow* window, World* world, vector<GameObject*>& entities);
};