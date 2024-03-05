#include "Block.hpp"

Block::Block(int x, int y, BlockData* bt) : Entity(x * BLOCKSIZE, y * BLOCKSIZE, bt->texture, BLOCKSIZE, BLOCKSIZE) {
	switchBlockType(bt);
}

void Block::switchBlockType(BlockData* bt) {
	type = bt;
	currentFrame.x = bt->x;
	currentFrame.y = bt->y;
	currentFrame.w = bt->w;
	currentFrame.h = bt->h;
	width = bt->w;
	height = bt->h;
}

bool pointCompare(int ax, int ay, int bx, int by) {
	return (ay == by) ? (ax < bx) : (ay < by);
}

bool blockCompare(Block* a, Block* b) {
	if (a->type->permissable == b->type->permissable) {
		return pointCompare(a->x, a->y, b->x, b->y);
	}
	return a->type->permissable;
}