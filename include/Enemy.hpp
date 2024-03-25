#pragma once

#include "GameObject.hpp"
#include "EnemyData.hpp"
#include "Player.hpp"
#include "utils.hpp"
#include "Bar.hpp"

void light(Uint8& r, Uint8& g, Uint8& b, Uint8& a);
void dark(Uint8& r, Uint8& g, Uint8& b, Uint8& a);

class Enemy : public GameObject {
public:
	int health;
	int recoveryFrames = 0;
	EnemyData* ed;
	Bar* healthBar;

	bool lighter = false;

	int invincibilityFrames = 0;

	~Enemy();
	Enemy(int px, int py, EnemyData* e, RenderWindow* window);
	void pixelEdit(void (*func)(Uint8&, Uint8&, Uint8&, Uint8&));
	void lighten();
	void darken();

	virtual bool draw(RenderWindow* window, World* world, vector<GameObject*>& entities);
};