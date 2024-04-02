#include "World.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "utils.hpp"
#include "WorldGenerator.hpp"

int index(int x, int y) {
	while (x < 0) {
		x += World::WORLDSIZE;
	} 
	while (y < 0) {
		y += World::WORLDSIZE;
	}
	return (y % World::WORLDSIZE) * World::WORLDSIZE + (x % World::WORLDSIZE);
}

World::World(RenderWindow* window, Player* player) {
	this->player = player;

	// LOAD STRUCTURES
	STRUCTYPES = {
		new StructureType("Entrance", 4, 1), 
		new StructureType("Exit", 4, 1), 
		new StructureType("Chest", 1, 0.3), 
		new StructureType("Enemy", 1, 0.7), 
	};
}

void World::loadBlocks(RenderWindow* window) {
	while (!BLOCKTYPES.empty()) {
		delete BLOCKTYPES[0];
		BLOCKTYPES.erase(BLOCKTYPES.begin());
	}

	activeTriggers.clear();

	activeTriggers.push_back({BOSS, false});

	// BLOCKTYPES.push_back(new BlockData("Void", window->loadTexture("res/gfx/void.png")));

	shared_ptr<SDL_Texture> grasstiles(window->loadTexture("res/gfx/grasstiles.png"), sdl_deleter());
	shared_ptr<SDL_Texture> props(window->loadTexture("res/gfx/Props_1.2.png"), sdl_deleter());

	BLOCKTYPES.push_back(new BlockData("DirtPath", grasstiles, 0, 5 * PIXELSIZE, PIXELSIZE, PIXELSIZE, 0.7, true));
	BLOCKTYPES.push_back(new BlockData("ShortPillar", props, PIXELSIZE * 4, PIXELSIZE * 2, PIXELSIZE, PIXELSIZE));
	BLOCKTYPES.push_back(new BlockData("SmallBush", grasstiles, 0, PIXELSIZE * 4, PIXELSIZE, PIXELSIZE));
	BLOCKTYPES.push_back(new BlockData("ChestSmallForwardClosed", props, PIXELSIZE * 5, 0, PIXELSIZE, PIXELSIZE));
	BLOCKTYPES.push_back(new BlockData("ChestSmallForwardOpen", props, PIXELSIZE * 5, PIXELSIZE, PIXELSIZE, PIXELSIZE));
	BLOCKTYPES.push_back(new BlockData("ChestSmallSideClosed", props, PIXELSIZE * 8, 0, PIXELSIZE, PIXELSIZE));
	BLOCKTYPES.push_back(new BlockData("ChestSmallSideOpen", props, PIXELSIZE * 8, PIXELSIZE, PIXELSIZE, 19));

	for (int x = 0; x < 3; x++) {
		for (int y = 0; y < 3; y++) {
			BLOCKTYPES.push_back(new BlockData("Grass" + to_string(x) + to_string(y), grasstiles, PIXELSIZE * x, PIXELSIZE * y, PIXELSIZE, PIXELSIZE, 0.65, true));
			BLOCKTYPES.push_back(new BlockData("GrassBlock" + to_string(x) + to_string(y), grasstiles, PIXELSIZE * (x + 6), PIXELSIZE * y, PIXELSIZE, PIXELSIZE));
			BLOCKTYPES.push_back(new BlockData("BrickArch" + to_string(x) + to_string(y), props, PIXELSIZE * x, PIXELSIZE * (y + 2), PIXELSIZE, PIXELSIZE, 1, (y != 2 || x == 1), (y != 2 || x == 1)));
			BLOCKTYPES.push_back(new BlockData("SmallTree" + to_string(x) + to_string(y), props, PIXELSIZE * (x + 15), PIXELSIZE * y, PIXELSIZE, PIXELSIZE, 1, (y != 2 || x != 1), (y != 2 || x != 1)));			
		}
		BLOCKTYPES.push_back(new BlockData("GrassRandom" + to_string(2 - x), grasstiles, PIXELSIZE * x, 3 * PIXELSIZE, PIXELSIZE, PIXELSIZE, 0.6, true));
		BLOCKTYPES.push_back(new BlockData("SidewaysSign" + to_string(2 - x), props, PIXELSIZE * 7, (x + 2) * PIXELSIZE, PIXELSIZE, PIXELSIZE, 0.6, !(x == 2), !(x == 2)));
	}

	for (int x = 0; x < 2; x++) {
		for (int y = 0; y < 2; y++) {
			BLOCKTYPES.push_back(new BlockData("GrassEdge" + to_string(x) + to_string(y), grasstiles, PIXELSIZE * (x + 6), PIXELSIZE * (y + 3), PIXELSIZE, PIXELSIZE, 0.68, true));
			BLOCKTYPES.push_back(new BlockData("GrassBlockEdge" + to_string(x) + to_string(y), grasstiles, PIXELSIZE * (x + 6), PIXELSIZE * (y + 5), PIXELSIZE, PIXELSIZE));
			BLOCKTYPES.push_back(new BlockData("SpawnPlatform" + to_string(x) + to_string(y), props, PIXELSIZE * x, PIXELSIZE * (y + 5), PIXELSIZE, PIXELSIZE, 0.78, true));
			BLOCKTYPES.push_back(new BlockData("BigBush" + to_string(x) + to_string(y), grasstiles, PIXELSIZE * (x + 1), PIXELSIZE * (y + 4), PIXELSIZE, PIXELSIZE));
		}
		BLOCKTYPES.push_back(new BlockData("DirtRandom" + to_string(x), grasstiles, (10 + x) * PIXELSIZE, 8 * PIXELSIZE, PIXELSIZE, PIXELSIZE, 0.68, true));		
		BLOCKTYPES.push_back(new BlockData("TallPillar" + to_string(1 - x), props, PIXELSIZE * 4, PIXELSIZE * x, PIXELSIZE, PIXELSIZE, 1, !(x == 1), !(x == 1)));
		BLOCKTYPES.push_back(new BlockData("DownRightSign" + to_string(1 - x), props, PIXELSIZE * 5, PIXELSIZE * (x + 2), PIXELSIZE, PIXELSIZE, 1, !(x == 1), !(x == 1)));
		BLOCKTYPES.push_back(new BlockData("DownLeftSign" + to_string(1 - x), props, PIXELSIZE * 5, PIXELSIZE * (x + 4), PIXELSIZE, PIXELSIZE, 1, !(x == 1), !(x == 1)));
		BLOCKTYPES.push_back(new BlockData("UpRightSign" + to_string(1 - x), props, PIXELSIZE * 6, PIXELSIZE * (x + 2), PIXELSIZE, PIXELSIZE, 1, !(x == 1), !(x == 1)));
		BLOCKTYPES.push_back(new BlockData("UpLeftSign" + to_string(1 - x), props, PIXELSIZE * 6, PIXELSIZE * (x + 4), PIXELSIZE, PIXELSIZE, 1, !(x == 1), !(x == 1)));
	}

	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 2; y++) {
			// NOTE: THE ONLY THING THAT HAS Y BEFORE X
			BLOCKTYPES.push_back(new BlockData("BrickBlock" + to_string(y) + to_string(x), props, PIXELSIZE * x, PIXELSIZE * y, PIXELSIZE, PIXELSIZE, 1, (y == 1) && (x > 1)));			
		}
	}

	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 5; y++) {
			bool hardOrNah = (y > 2) && (x > 0) && (x < 3);
			BLOCKTYPES.push_back(new BlockData("BigTree" + to_string(x) + to_string(y), props, PIXELSIZE * (x + 11), PIXELSIZE * y, PIXELSIZE, PIXELSIZE, 1, !hardOrNah, !hardOrNah));			
		}
	}

	sort(BLOCKTYPES.begin(), BLOCKTYPES.end(), dataCompare);
}

bool World::isSafePosition(int midX, int midY) {
	bool safePosition = true;
	for (int y = 0; y < 2; y++) {
		int spawnX = structures[y].x / Block::BLOCKSIZE;
		int spawnY = structures[y].y / Block::BLOCKSIZE;

		if ((abs(spawnX - midX) < 15 || abs(spawnX - midX) > World::WORLDSIZE - 15) && (abs(spawnY - midY) < 15 || abs(spawnY - midY) > World::WORLDSIZE - 15)) {
			safePosition = false;
			break;
		}
	}
	if (!safePosition) {
		return false;
	}
	for (Block* block : blocks) {
		if ((!block->type->permissable || !block->isSolidGrass()) && abs(block->x / Block::BLOCKSIZE - midX) <= 3 && abs(block->y / Block::BLOCKSIZE - midY) <= 3) {
			safePosition = false;
			break;
		}
	}

	return safePosition;
}

void World::edgeCleanup(string toEdge) {
	for (int x = 0; x < WORLDSIZE; x++) {
		for (int y = 0; y < WORLDSIZE; y++) {
			Block* b = blocks[index(x, y)];
			// cout << b->type->name.substr(0, toEdge.size()) << endl;
			if (b->type->name == toEdge + "11") {
				int xdelta = 1;
				int ydelta = 1;
				int edgeDiffs = 0;

				// cout << ">???\n";

				if (blocks[index(x + 1, y)]->type->name.substr(0, toEdge.size()) != toEdge) {
					xdelta = 2;
					edgeDiffs++;
				}
				if (blocks[index(x - 1, y)]->type->name.substr(0, toEdge.size()) != toEdge) {
					xdelta = 0;
					edgeDiffs++;
				}
				if (blocks[index(x, y + 1)]->type->name.substr(0, toEdge.size()) != toEdge) {
					ydelta = 2;
					edgeDiffs++;
				}
				if (blocks[index(x, y - 1)]->type->name.substr(0, toEdge.size()) != toEdge) {
					ydelta = 0;
					edgeDiffs++;
				}
				if (xdelta == 1 && ydelta == 1) {
					if (blocks[index(x + 1, y + 1)]->type->name.substr(0, toEdge.size()) != toEdge) {
						b->switchBlockType(getBlockData(toEdge + "Edge11"));
					}
					if (blocks[index(x + 1, y - 1)]->type->name.substr(0, toEdge.size()) != toEdge) {
						b->switchBlockType(getBlockData(toEdge + "Edge01"));
					}
					if (blocks[index(x - 1, y + 1)]->type->name.substr(0, toEdge.size()) != toEdge) {
						b->switchBlockType(getBlockData(toEdge + "Edge10"));
					}
					if (blocks[index(x - 1, y - 1)]->type->name.substr(0, toEdge.size()) != toEdge) {
						b->switchBlockType(getBlockData(toEdge + "Edge00"));
					}
				}
				else {
					b->switchBlockType(getBlockData(toEdge + to_string(ydelta) + to_string(xdelta)));
				}
			}
		}
	}
}

void World::tempEdgeCleanup(string toEdge, string toReplace) {
	for (int x = 0; x < WORLDSIZE; x++) {
		for (int y = 0; y < WORLDSIZE; y++) {
			Block* b = blocks[index(x, y)];
			if (b->type->name == toEdge + "11") {
				int xDiffs = 0;
				int yDiffs = 0;

				if (blocks[index(x + 1, y)]->type->name.substr(0, toEdge.size()) != toEdge) {
					xDiffs++;
				}
				if (blocks[index(x - 1, y)]->type->name.substr(0, toEdge.size()) != toEdge) {
					xDiffs++;
				}
				if (blocks[index(x, y + 1)]->type->name.substr(0, toEdge.size()) != toEdge) {
					yDiffs++;
				}
				if (blocks[index(x, y - 1)]->type->name.substr(0, toEdge.size()) != toEdge) {
					yDiffs++;
				}

				if (xDiffs == 2 || yDiffs == 2) {
					b->switchBlockType(getBlockData(toReplace));
				}
			}
		}
	}
}


bool World::notNearStructure(int x, int y) {
	for (Structure& s : structures) {
		if (abs(1.0 * s.x / Block::BLOCKSIZE - x) < 5 * s.type->size && abs(1.0 * s.y / Block::BLOCKSIZE - y) < 5 * s.type->size) {
			return false;
		}
	}
	return true;
}

BlockData* World::getBlockData(string s) {
    int bottom = 0;
    int top = BLOCKTYPES.size() - 1;
    while (top >= bottom) {
        int index = bottom + (top - bottom) / 2;
        if (BLOCKTYPES[index]->name == s) {
        	return BLOCKTYPES[index];
        }
        if (BLOCKTYPES[index]->name > s) {
            top = index - 1;
        }
        else {
            bottom = index + 1;
        }
    }
    cout << "ERROR: COULDN'T FIND " << s << endl;
    return nullptr;
}

/*
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
*/

bool World::blockInRect(Block* b, int x, int y, int w, int h) {
	int bs = Block::BLOCKSIZE;
	return (b->x + bs > x) && (b->x - bs < x + w) && (b->y + bs > y) && (b->y - bs < y + h);
}

void World::activateTrigger(TRIGGER t) {
	for (Block* b : blocks) {
		if (b->word == t) {
			b->active = !b->active;
		}
	}
}

void World::draw(RenderWindow* window, vector<GameObject*>& entities, bool front) {
	bool extendXP;
	bool extendXN;
	bool extendYP;
	bool extendYN;

	fixPreLoop(window, extendXP, extendXN, extendYP, extendYN);

	// cout << "World's player: " << player << endl;

	if (!front) {
		int xMin = ((player->x - RenderWindow::WIDTH / window->zoom) / Block::BLOCKSIZE) - 1;
		int xMax = ((player->x + RenderWindow::WIDTH / window->zoom) / Block::BLOCKSIZE) + 1;
		int yMin = ((player->y - RenderWindow::HEIGHT / window->zoom) / Block::BLOCKSIZE) - 1;
		int yMax = ((player->y + RenderWindow::HEIGHT / window->zoom) / Block::BLOCKSIZE) + 1;

		// cout << "xMin: " << xMin << " xMax: " << xMax << " yMin: " << yMin << " yMax: " << yMax << endl;
		for (int x = xMin; x < xMax; x++) {
			// cout << "x: " << x << " ";
			// int actualX = x - int(floor(x / World::WORLDLENGTH)) * World::WORLDLENGTH;
			for (int y = yMin; y < yMax; y++) {
				// cout << "y: " << y << " ";
				// int actualY = y - int(floor(y / World::WORLDLENGTH)) * World::WORLDLENGTH;
				Block* b = blocks[index(y, x)];
				loopPreFix(b, extendXP, extendXN, extendYP, extendYN);
				window->render(b);
				loopPostFix(b);
				// cout << index(y, x) << " ";
			}
			// cout << endl;
		}
		// cout << "cock" << endl;
	}
	// cout << "dick\n";

	// cout << "OtherRender\n";
	for (int x = WORLDSIZE * WORLDSIZE; x < blocks.size(); x++) {
		Block* b = blocks[x];
		if (b->type->front == front && b->active) {
			loopPreFix(b, extendXP, extendXN, extendYP, extendYN);
			window->render(b);
			loopPostFix(b);
		}
	}

	// cout << "TriggerDetect\n";
	int xDiff = player->x - structures[1].x + player->show_width / 2;
	int yDiff = player->y - structures[1].y + player->show_height / 2;
	// cout << xDiff * xDiff + yDiff * yDiff << endl;
	if (xDiff * xDiff + yDiff * yDiff < 160000 && !activeTriggers[0].second) { // Distance is less than 400
		activeTriggers[0].second = true;
		activateTrigger(activeTriggers[0].first);
		shrub = new Enemy(WORLDLENGTH / 2 - Block::BLOCKSIZE / 2, WORLDLENGTH / 2 - 3 * Block::BLOCKSIZE, boss, window);
		entities.push_back(shrub);
	}
}

bool World::collides(GameObject* object) {
	SDL_Rect gobj = object->getRect();
	const SDL_Rect* gstar = &gobj;
	SDL_Rect actual = {0, 0, 0, 0};
	SDL_Rect* intersect = &actual;
	/*
	bool xStop = false;
	bool yStop = false;
	// */

	bool collided = false;

	for (Block* b : blocks) {
		//  && blockInRect(b, object->x - Block::BLOCKSIZE, object->y - Block::BLOCKSIZE, object->show_width + Block::BLOCKSIZE * 2, object->show_height + Block::BLOCKSIZE * 2)
		if (!b->type->permissable && b->active) {
			// /*
			SDL_Rect bobj = b->getRect();
			const SDL_Rect* bstar = &bobj;

			gobj = object->getRect();
			if (SDL_IntersectRect(gstar, bstar, intersect) == SDL_TRUE) {
				collided = true;

				if (actual.w < actual.h) {
					if (actual.x > gobj.x) {
						object->x += -actual.w;
					}
					else {
						object->x += actual.w;
					}
				}
				else {
					if (actual.y > gobj.y) {
						object->y += -actual.h;
					}
					else {
						object->y += actual.h;
					}
				}

				/*
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
				*/
			}
			// */
		}
	}
	/*
	if (xStop) {
		object->xvel = 0;
	}
	if (yStop) {
		object->yvel = 0;
	}
	// */
	// return xStop || yStop;
	return collided;
}