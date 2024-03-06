#include "World.hpp"

int index(int x, int y) {
	while (x < 0) {
		x += World::WORLDSIZE;
	} 
	while (y < 0) {
		y += World::WORLDSIZE;
	}
	return (y % World::WORLDSIZE) * World::WORLDSIZE + (x % World::WORLDSIZE);
}

World::World(RenderWindow* window) {
	// LOAD STRUCTURES
	STRUCTYPES = {
		new StructureType("Entrance", 3, 1), 
		new StructureType("Enemy", 1, 1), 
		// new StructureType("Exit", 3, 1), 
	};

	// LOAD BLOCKS
	BLOCKTYPES.push_back(new BlockData("Void", window->loadTexture("res/gfx/void.png")));

	SDL_Texture* grasstiles = window->loadTexture("res/gfx/grasstiles.png");

	BLOCKTYPES.push_back(new BlockData("DirtPath", grasstiles, 0, 400, 80, 80, true));
	for (int x = 0; x < 3; x++) {
		for (int y = 0; y < 3; y++) {
			BLOCKTYPES.push_back(new BlockData("Grass" + to_string(x) + to_string(y), grasstiles, 80 * x, 80 * y, 80, 80, true));
		}
		BLOCKTYPES.push_back(new BlockData("GrassRandom" + to_string(x), grasstiles, 80 * x, 240, 80, 80, true));
	}

	for (int x = 0; x < 2; x++) {
		for (int y = 0; y < 2; y++) {
			BLOCKTYPES.push_back(new BlockData("GrassEdge" + to_string(x) + to_string(y), grasstiles, 80 * x + 480, 80 * y + 240, 80, 80, true));
		}
		BLOCKTYPES.push_back(new BlockData("DirtRandom" + to_string(x), grasstiles, 240, 80 * x + 480, 80, 80, true));		
	}

	sort(BLOCKTYPES.begin(), BLOCKTYPES.end(), dataCompare);

	// GENERATE WORLD
	auto seed = (unsigned) time(NULL);
	// auto seed = 1709637698;
	cout << "SEED: " << seed << endl;

	PerlinNoise pn = PerlinNoise(seed);
	srand(seed);
	random(); // FIRST RAND CALL IS LINEAR TO SEED [BURN IT]
	// double z = random();
	// cout << z << endl;

	// GENERATE STRUCTURE LOCATIONS
	if (random() > 0.5) {
		structures.push_back(Structure(STRUCTYPES[0], random() * WORLDLENGTH, 0));
	}
	else {
		structures.push_back(Structure(STRUCTYPES[0], 0, random() * WORLDLENGTH));		
	}

	int numStructs = WORLDSIZE / 100.0 + random() * WORLDSIZE / 100.0;
	for (int x = 0; x < numStructs; x++) {
		int x1 = random() * WORLDLENGTH;
		int y1 = random() * WORLDLENGTH;

		int xdelta = 0;
		int ydelta = 0;

		for (Structure& s : structures) {
			float power = s.type->rarity * SEPERATION_FORCE_FACTOR / sqrt(pow(x1 - s.x, 2) + pow(y1 - s.y, 2));
			float angle = angleBetween(x1, y1, s.x, s.y);

			xdelta -= cos(angle);
			ydelta += sin(angle);
		}

		x1 += xdelta;
		y1 += ydelta;

		x1 = x1 % WORLDLENGTH;
		y1 = y1 % WORLDLENGTH;

		structures.push_back(Structure(STRUCTYPES[1], x1, y1));
	}

	// PLACE DIRT AND GRASS
	for (int x = 0; x < WORLDSIZE; x++) {
		float xangle = M_PI * 2.0 * (x + 1) / (WORLDSIZE + 1);
		for (int y = 0; y < WORLDSIZE; y++) {
			float yangle = M_PI * 1.0 * (y + 1) / (WORLDSIZE + 1);
			if (pn.noise(sin(xangle) * cos(yangle), sin(xangle) * sin(yangle), cos(xangle)) > 0.6) {
				blocks.push_back(new Block(x, y, getBlockData("DirtPath")));
			}
			else {
				blocks.push_back(new Block(x, y, getBlockData("Grass11")));
			}
		}
	}

	// /*
	// DIRT EDGE CLEANUP
	for (int x = 0; x < WORLDSIZE; x++) {
		for (int y = 0; y < WORLDSIZE; y++) {
			Block* b = blocks[index(x, y)];
			if (b->type->name == "Grass11") {
				int xdelta = 1;
				int ydelta = 1;

				// cout << ">???\n";

				if (blocks[index(x + 1, y)]->type->name == "DirtPath") {
					xdelta = 2;
				}
				if (blocks[index(x - 1, y)]->type->name == "DirtPath") {
					xdelta = 0;
				}
				if (blocks[index(x, y + 1)]->type->name == "DirtPath") {
					ydelta = 2;
				}
				if (blocks[index(x, y - 1)]->type->name == "DirtPath") {
					ydelta = 0;
				}
				if (xdelta == 1 && ydelta == 1) {
					if (blocks[index(x + 1, y + 1)]->type->name == "DirtPath") {
						b->switchBlockType(getBlockData("GrassEdge11"));
					}
					if (blocks[index(x + 1, y - 1)]->type->name == "DirtPath") {
						b->switchBlockType(getBlockData("GrassEdge01"));
					}
					if (blocks[index(x - 1, y + 1)]->type->name == "DirtPath") {
						b->switchBlockType(getBlockData("GrassEdge10"));
					}
					if (blocks[index(x - 1, y - 1)]->type->name == "DirtPath") {
						b->switchBlockType(getBlockData("GrassEdge00"));
					}
				}
				else {
					b->switchBlockType(getBlockData("Grass" + to_string(ydelta) + to_string(xdelta)));
				}
			}
		}
	}

	for (int x = 0; x < WORLDSIZE; x++) {
		for (int y = 0; y < WORLDSIZE; y++) {
			if (blocks[index(x, y)]->type->name == "Grass11" && random() > 0.6) {
				blocks[index(x, y)]->switchBlockType(getBlockData("GrassRandom" + to_string(int(3 * random()))));
			}
			else if (blocks[index(x, y)]->type->name == "DirtPath" && random() > 0.6) {
				blocks[index(x, y)]->switchBlockType(getBlockData("DirtRandom" + to_string(int(2 * random()))));
			}
		}
	}

	// */

	blocks.push_back(new Block(2, 2, getBlockData("Void")));
	// SORT BY LOW TO HIGH (Unecessary?)
	// sort(blocks.begin(), blocks.end(), blockCompare);
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

void World::draw(RenderWindow* window) {
	int x = window->x;
	int y = window->y;
	int w = RenderWindow::WIDTH / window->zoom;
	int h = RenderWindow::HEIGHT / window->zoom;

	// cout << x << " "<< y << " "<< w << " "<< h << endl;

	for (Block* b : blocks) {
		// int xChange = 0;
		// int yChange = 0;
		/*
		for (int z = 0; z < 9; z++) {
			int xChange = (z % 3 - 1) * WORLDLENGTH;
			int yChange = (z / 3 - 1) * WORLDLENGTH;

			if (blockInRect(b, x + xChange, y + yChange, w, h)) {
				b->x -= xChange;
				b->y -= yChange;

				window->render(b);
				
				b->x += xChange;
				b->y += yChange;
			}
		}
		*/
		window->render(b);
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
		if (!b->type->permissable && blockInRect(b, object->x - Block::BLOCKSIZE, object->y - Block::BLOCKSIZE, object->show_width + Block::BLOCKSIZE * 2, object->show_height + Block::BLOCKSIZE * 2)) {
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