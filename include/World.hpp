#pragma once

#include "Block.hpp"
#include "RenderWindow.hpp"
#include "GameObject.hpp"
#include "PerlinNoise.hpp"

#include <chrono>
#include <algorithm>

using namespace std;

int index(int x, int y);

class World {
public:
	static const int WORLDSIZE = 20;
	vector<BlockData*> BLOCKTYPES;
	vector<Block*> blocks;

	World(RenderWindow* window);
	BlockData* getBlockData(string s);
	void draw(RenderWindow* window);
	bool collides(GameObject* object);
	// Block* blockAt(int x, int y);
	static bool blockInRect(Block* b, int x, int y, int w, int h);
};