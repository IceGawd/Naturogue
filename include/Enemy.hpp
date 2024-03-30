#pragma once

#include "GameObject.hpp"
#include "EnemyData.hpp"
#include "Player.hpp"
#include "utils.hpp"
#include "Bar.hpp"
#include "Block.hpp"

void light(Uint8& r, Uint8& g, Uint8& b, Uint8& a);
void dark(Uint8& r, Uint8& g, Uint8& b, Uint8& a);

class Enemy : public GameObject {
public:
	int health;
	int recoveryFrames = 0;
	EnemyData* ed;
	Bar* healthBar;

	int randoSampledX = 0;
	int randoSampledY = 0;

	bool lighter = false;
	bool active = false;
	bool rage = false;
	bool findHidingSpot = true;
	Block* hidingSpot = nullptr;
	Block* onTopOf = nullptr;
	float rageMeter = 0;

	DIRECTION facing;

	vector<Enemy*> group;

	int invincibilityFrames = 0;

	virtual ~Enemy();
	Enemy(int px, int py, EnemyData* e, RenderWindow* window);
	void pixelEdit(void (*func)(Uint8&, Uint8&, Uint8&, Uint8&));
	void lighten();
	void darken();

	virtual bool draw(RenderWindow* window, World* world, vector<GameObject*>& entities);
};