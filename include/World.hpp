#pragma once

#include "Block.hpp"
#include "RenderWindow.hpp"
#include "GameObject.hpp"
#include "PerlinNoise.hpp"
#include "Structure.hpp"
#include "utils.hpp"

#include <chrono>
#include <algorithm>

using namespace std;

int index(int x, int y);

struct Player;

class World {
public:
	static const int WORLDSIZE = 100;
	static const int PIXELSIZE = 16;
	static constexpr int WORLDLENGTH = WORLDSIZE * Block::BLOCKSIZE;
	static const int SEPERATION_FORCE_FACTOR = 500;
	vector<BlockData*> BLOCKTYPES;
	vector<Block*> blocks;
	vector<StructureType*> STRUCTYPES;
	vector<Structure> structures;
	Player* player;

	World(RenderWindow* window, Player* player);
	BlockData* getBlockData(string s);
	void draw(RenderWindow* window);
	bool collides(GameObject* object);
	// Block* blockAt(int x, int y);
	static bool blockInRect(Block* b, int x, int y, int w, int h);
};