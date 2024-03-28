#pragma once

#include <vector>

#include "Entity.hpp"
#include "BlockData.hpp"
#include "utils.hpp"

class Block : public Entity {
public:
	const static int BLOCKSIZE = 80;
	BlockData* type;
	bool active = true;
	TRIGGER word;

	Block(int x, int y, BlockData* bt);
	Block(pair<int, int> coords, BlockData* bt);
	Block(pair<int, int> coords, BlockData* bt, TRIGGER w);
	void switchBlockType(BlockData* bt);
	bool isSolidGrass();
};

bool pointCompare(int ax, int ay, int bx, int by);
bool blockCompare(Block* a, Block* b);