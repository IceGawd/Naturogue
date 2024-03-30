#include "Block.hpp"

Block::Block(int x, int y, BlockData* bt, bool f) : Entity(x * BLOCKSIZE, y * BLOCKSIZE, bt->texture, BLOCKSIZE, BLOCKSIZE) {
	switchBlockType(bt);
	flip = f;
}

Block::Block(pair<int, int> coords, BlockData* bt) : Entity(coords.first * BLOCKSIZE, coords.second * BLOCKSIZE, bt->texture, BLOCKSIZE, BLOCKSIZE) {
	switchBlockType(bt);
}

Block::Block(pair<int, int> coords, BlockData* bt, TRIGGER w) : Entity(coords.first * BLOCKSIZE, coords.second * BLOCKSIZE, bt->texture, BLOCKSIZE, BLOCKSIZE) {
	switchBlockType(bt);
	word = w;
	active = false;
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

bool Block::isSolidGrass() {
	return type->name.substr(0, 7) == "Grass11" || type->name.substr(0, 11) == "GrassRandom";
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