#pragma once

#include <vector>

#include "Entity.hpp"
#include "BlockData.hpp"

class Block : public Entity {
public:
	const static int BLOCKSIZE = 80;
	BlockData* type;

	Block(int x, int y, BlockData* bt);
	void switchBlockType(BlockData* bt);
};

bool pointCompare(int ax, int ay, int bx, int by);
bool blockCompare(Block* a, Block* b);