#pragma once

#include "World.hpp"
#include "Player.hpp"
#include "Enemy.hpp"

inline pair<int, int> flippedIndex(int num, vector<pair<int, int>>& degree45, int subtract = 0);
void generateWorld(World* world, RenderWindow* window, Player* player, vector<GameObject*>& entities, vector<EnemyData*> enemyDatas);

struct SignInfo {
	string name;
	int size;
	int x;
	int y;
};