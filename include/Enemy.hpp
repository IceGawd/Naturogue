#pragma once

#include "GameObject.hpp"
#include "EnemyData.hpp"
#include "World.hpp"
#include "Player.hpp"

class Enemy : public GameObject {
public:
	int health;
	int recoveryFrames = 0;
	EnemyData* ed;

	Enemy(int px, int py, EnemyData* e);

	virtual bool draw(RenderWindow* window, World* world, vector<GameObject*>& entities);
};