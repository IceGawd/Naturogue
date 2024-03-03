#pragma once

#include <vector>

#include "Entity.hpp"
#include "BlockData.hpp"

class Block : public Entity {
public:
	const static int BLOCKSIZE = 100;
	Block(int x, int y, BlockData bt);
};

bool pointCompare(int ax, int ay, int bx, int by);
bool blockCompare(Block* a, Block* b);