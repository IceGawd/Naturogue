#pragma once

#include "Block.hpp"
#include "RenderWindow.hpp"
#include "GameObject.hpp"

#include <algorithm>

using namespace std;

class World {
public:
	vector<BlockData> BLOCKTYPES;
	vector<Block*> blocks;

	World(RenderWindow* window);
	void draw(RenderWindow* window);
	bool collides(GameObject* object);
	Block* blockAt(int x, int y);
	static bool blockInRect(Block* b, int x, int y, int w, int h);
};