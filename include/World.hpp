#pragma once

#include "Block.hpp"
#include "RenderWindow.hpp"
#include "GameObject.hpp"
#include "PerlinNoise.hpp"
#include "Structure.hpp"
#include "utils.hpp"
#include "EnemyData.hpp"

#include <chrono>
#include <algorithm>

using namespace std;

int index(int x, int y);

struct Player;
struct Enemy;

class World {
public:
	static const int WORLDSIZE = 200;
	static const int PIXELSIZE = 16;
	static constexpr int WORLDLENGTH = WORLDSIZE * Block::BLOCKSIZE;
	static const int SEPERATION_FORCE_FACTOR = 1000;
	vector<BlockData*> BLOCKTYPES;
	vector<Block*> blocks;
	vector<StructureType*> STRUCTYPES;
	vector<Structure> structures;
	vector<pair<TRIGGER, bool>> activeTriggers; 
	Player* player;
	EnemyData* boss;
	Enemy* shrub = nullptr;
	int renderSize = 5000;

	World(RenderWindow* window, Player* player, vector<GameObject*>& entities, vector<EnemyData*> enemyDatas);
	void edgeCleanup(string toEdge);
	bool notNearStructure(int x, int y);
	BlockData* getBlockData(string s);
	void draw(RenderWindow* window, vector<GameObject*>& entities, bool front);
	bool collides(GameObject* object);
	// Block* blockAt(int x, int y);
	static bool blockInRect(Block* b, int x, int y, int w, int h);
	void activateTrigger(TRIGGER t);
};