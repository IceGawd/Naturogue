#include "World.hpp"

World::World(RenderWindow* window) {
	BLOCKTYPES.push_back({"Void", window->loadTexture("res/gfx/void.png")});

	blocks.push_back(new Block(2, 2, BLOCKTYPES[0]));

	// SORT BY LOW TO HIGH (Unecessary?)
	// sort(blocks.begin(), blocks.end(), blockCompare);
}

Block* World::blockAt(int x, int y) {
	x = x / Block::BLOCKSIZE * Block::BLOCKSIZE;
	y = y / Block::BLOCKSIZE * Block::BLOCKSIZE;

	float jump = blocks.size() / 2;
	float index = jump;

	Block* b = blocks.at((int) index);
	while (jump > 1) {
		jump = jump / 2;
		if (pointCompare(b->x, b->y, x, y)) {
			index += jump;
		}
		else if (pointCompare(x, y, b->x, b->y)) {
			index -= jump;
		}
		else {
			break;
		}
		b = blocks.at((int) index);
	}

	if (b->x == x && b->y == y) {
		return b;
	}
	else {
		return nullptr;
	}
}

bool World::blockInRect(Block* b, int x, int y, int w, int h) {
	int bs = Block::BLOCKSIZE;
	return (b->x + bs > x) && (b->x - bs < x + w) && (b->y + bs > y) && (b->y - bs < y + h);
}

void World::draw(RenderWindow* window) {
	int x = window->x;
	int y = window->y;
	int w = RenderWindow::WIDTH * window->zoom;
	int h = RenderWindow::HEIGHT * window->zoom;

	for (Block* b : blocks) {
		if (blockInRect(b, x, y, w, h)) {
			window->render(b);
		}
	}
}


bool World::collides(GameObject* object) {
	SDL_Rect gobj = object->getRect();
	const SDL_Rect* gstar = &gobj;
	SDL_Rect actual = {0, 0, 0, 0};
	SDL_Rect* intersect = &actual;
	bool xStop = false;
	bool yStop = false;
	for (Block* b : blocks) {
		if (blockInRect(b, object->x - Block::BLOCKSIZE, object->y - Block::BLOCKSIZE, Block::BLOCKSIZE * 3, Block::BLOCKSIZE * 3)) {
			SDL_Rect bobj = b->getRect();
			const SDL_Rect* bstar = &bobj;

			gobj = object->getRect();
			if (SDL_IntersectRect(gstar, bstar, intersect) == SDL_TRUE) {
				if (abs(intersect->w) <= ceil(abs(object->xvel))) {
					xStop = true;
					if (intersect->x == gobj.x) {
						object->x = b->x + Block::BLOCKSIZE;
					}
					else {
						object->x = b->x - object->show_width;
					}
				}
				if (abs(intersect->h) <= ceil(abs(object->yvel))) {
					yStop = true;
					if (intersect->y == gobj.y) {
						object->y = b->y + Block::BLOCKSIZE;
					}
					else {
						object->y = b->y - object->show_height;
					}
				}
			}
		}
	}
	if (xStop) {
		object->xvel = 0;
	}
	if (yStop) {
		object->yvel = 0;
	}
	return xStop || yStop;
}