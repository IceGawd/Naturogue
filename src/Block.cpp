#include "Block.hpp"

Block::Block(int x, int y, BlockData bt) : Entity(x * BLOCKSIZE, y * BLOCKSIZE, bt.texture, BLOCKSIZE, BLOCKSIZE) {
}

bool pointCompare(int ax, int ay, int bx, int by) {
	return (ay == by) ? (ax < bx) : (ay < by);
}

bool blockCompare(Block* a, Block* b) {
	return pointCompare(a->x, a->y, b->x, b->y);
}