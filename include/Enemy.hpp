#pragma once

#include "GameObject.hpp"
#include "EnemyData.hpp"
#include "Player.hpp"
#include "utils.hpp"

class Enemy : public GameObject {
public:
	int health;
	int recoveryFrames = 0;
	EnemyData* ed;

	int invincibilityFrames = 0;

	Enemy(int px, int py, EnemyData* e);

	virtual bool draw(RenderWindow* window, World* world, vector<GameObject*>& entities);
};