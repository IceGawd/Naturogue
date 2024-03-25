#include "World.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "utils.hpp"

int index(int x, int y) {
	while (x < 0) {
		x += World::WORLDSIZE;
	} 
	while (y < 0) {
		y += World::WORLDSIZE;
	}
	return (y % World::WORLDSIZE) * World::WORLDSIZE + (x % World::WORLDSIZE);
}

World::World(RenderWindow* window, Player* player, vector<GameObject*>& entities, vector<EnemyData*> enemyDatas) {
	this->player = player;
	
	// LOAD STRUCTURES
	STRUCTYPES = {
		new StructureType("Entrance", 3, 1), 
		new StructureType("Exit", 3, 1), 
		new StructureType("Enemy", 1, 1), 
	};

	// LOAD BLOCKS
	BLOCKTYPES.push_back(new BlockData("Void", window->loadTexture("res/gfx/void.png")));

	SDL_Texture* grasstiles = window->loadTexture("res/gfx/grasstiles.png");
	SDL_Texture* props = window->loadTexture("res/gfx/Props_1.2.png");

	BLOCKTYPES.push_back(new BlockData("DirtPath", grasstiles, 0, 5 * PIXELSIZE, PIXELSIZE, PIXELSIZE, true));
	BLOCKTYPES.push_back(new BlockData("TallPillar1", props, PIXELSIZE * 4, 0, PIXELSIZE, PIXELSIZE, true, true));
	BLOCKTYPES.push_back(new BlockData("TallPillar0", props, PIXELSIZE * 4, PIXELSIZE, PIXELSIZE, PIXELSIZE, false));
	BLOCKTYPES.push_back(new BlockData("ShortPillar", props, PIXELSIZE * 4, PIXELSIZE * 2, PIXELSIZE, PIXELSIZE, false));

	for (int x = 0; x < 3; x++) {
		for (int y = 0; y < 3; y++) {
			BLOCKTYPES.push_back(new BlockData("Grass" + to_string(x) + to_string(y), grasstiles, PIXELSIZE * x, PIXELSIZE * y, PIXELSIZE, PIXELSIZE, true));
			BLOCKTYPES.push_back(new BlockData("GrassBlock" + to_string(x) + to_string(y), grasstiles, PIXELSIZE * (x + 6), PIXELSIZE * y, PIXELSIZE, PIXELSIZE, false));
			BLOCKTYPES.push_back(new BlockData("BrickArch" + to_string(x) + to_string(y), props, PIXELSIZE * x, PIXELSIZE * (y + 2), PIXELSIZE, PIXELSIZE, (y != 2 || x == 1), (y != 2 || x == 1)));
		}
		BLOCKTYPES.push_back(new BlockData("GrassRandom" + to_string(x), grasstiles, PIXELSIZE * x, 3 * PIXELSIZE, PIXELSIZE, PIXELSIZE, true));
	}

	for (int x = 0; x < 2; x++) {
		for (int y = 0; y < 2; y++) {
			BLOCKTYPES.push_back(new BlockData("GrassEdge" + to_string(x) + to_string(y), grasstiles, PIXELSIZE * (x + 6), PIXELSIZE * (y + 3), PIXELSIZE, PIXELSIZE, true));
			BLOCKTYPES.push_back(new BlockData("GrassBlockEdge" + to_string(x) + to_string(y), grasstiles, PIXELSIZE * (x + 6), PIXELSIZE * (y + 5), PIXELSIZE, PIXELSIZE, false));
			BLOCKTYPES.push_back(new BlockData("SpawnPlatform" + to_string(x) + to_string(y), props, PIXELSIZE * x, PIXELSIZE * (y + 5), PIXELSIZE, PIXELSIZE, true));
		}
		BLOCKTYPES.push_back(new BlockData("DirtRandom" + to_string(x), grasstiles, (10 + x) * PIXELSIZE, 8 * PIXELSIZE, PIXELSIZE, PIXELSIZE, true));		
	}

	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 2; y++) {
			BLOCKTYPES.push_back(new BlockData("BrickBlock" + to_string(x) + to_string(y), props, PIXELSIZE * x, PIXELSIZE * y, PIXELSIZE, PIXELSIZE));			
		}
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
		structures.push_back(Structure(STRUCTYPES[0], (int) ((random() * WORLDLENGTH) / Block::BLOCKSIZE) * Block::BLOCKSIZE, 0));
	}
	else {
		structures.push_back(Structure(STRUCTYPES[0], 0, (int) ((random() * WORLDLENGTH) / Block::BLOCKSIZE) * Block::BLOCKSIZE));		
	}

	structures.push_back(Structure(STRUCTYPES[1], WORLDLENGTH / 2, WORLDLENGTH / 2));

	int numStructs = WORLDSIZE / 10.0 + random() * WORLDSIZE / 10.0;
	for (int x = 0; x < numStructs; x++) {
		int x1 = random() * WORLDLENGTH;
		int y1 = random() * WORLDLENGTH;

		int xdelta = 0;
		int ydelta = 0;

		for (int n = 1; n < 10; n++) {
			for (Structure& s : structures) {
				int x2 = s.x;
				int y2 = s.y;

				if (abs(x1 - x2) > 4000) {
					if (x1 > x2) {
						x2 += 8000;
					}
					else {
						x2 -= 8000;
					}
				}
				if (abs(y1 - y2) > 4000) {
					if (y1 > y2) {
						y2 += 8000;
					}
					else {
						y2 -= 8000;
					}
				}

				float power = s.type->rarity * SEPERATION_FORCE_FACTOR / sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
				float angle = pointAngleBetween(x1, y1, x2, y2);

				xdelta += power * cos(angle);
				ydelta -= power * sin(angle);

				/*
				cout << "power: " << power << endl;
				cout << "angle: " << angle * 180 / M_PI << endl;
				cout << "x1: " << x1 << " y1: " << y1 << endl;
				cout << "x2: " << x2 << " y2: " << y2 << endl;
				cout << "s.x: " << s.x << " s.y: " << s.y << endl;
				cout << "power * cos(angle): " << power * cos(angle) << " -power * sin(angle): " << -power * sin(angle) << endl;
				*/
			}
			xdelta /= n;
			ydelta /= n;

			x1 += xdelta;
			y1 += ydelta;

			x1 -= int(floor(x1 / WORLDLENGTH)) * WORLDLENGTH;
			y1 -= int(floor(y1 / WORLDLENGTH)) * WORLDLENGTH;

			if (xdelta * ydelta < 10) {
				break;
			}
		}

		structures.push_back(Structure(STRUCTYPES[2], x1, y1));
	}

	// PLACE DIRT AND GRASS
	for (int x = 0; x < WORLDSIZE; x++) {
		float xangle = M_PI * 2.0 * (x + 1) / (WORLDSIZE + 1);
		for (int y = 0; y < WORLDSIZE; y++) {
			float yangle = M_PI * 2.0 * (y + 1) / (WORLDSIZE + 1);
			// /*
			if (pn.noise(sin(xangle) * cos(yangle), sin(xangle) * sin(yangle), cos(xangle)) > 0.7) {
				// blocks.push_back(new Block(x, y, getBlockData("DirtPath")));
				blocks.push_back(new Block(x, y, getBlockData("GrassBlock11")));
			}
			else {
				blocks.push_back(new Block(x, y, getBlockData("Grass11")));
			}
			// */
			/*
			if (x > WORLDSIZE / 2 && y > WORLDSIZE / 2) {
				blocks.push_back(new Block(x, y, getBlockData("DirtPath")));
			}
			else {
				blocks.push_back(new Block(x, y, getBlockData("Grass11")));
			}
			*/
		}
	}

	// /*
	// DIRT EDGE CLEANUP
	edgeCleanup("Grass");
	edgeCleanup("GrassBlock");

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

	/*
	blocks.push_back(new Block(2, 2, getBlockData("Void")));
	blocks.push_back(new Block(2, 3, getBlockData("Void")));
	blocks.push_back(new Block(3, 3, getBlockData("Void")));
	blocks.push_back(new Block(3, 2, getBlockData("Void")));
	*/
	// SORT BY LOW TO HIGH (Unecessary?)
	// sort(blocks.begin(), blocks.end(), blockCompare);

	for (Structure& s : structures) {
		cout << s.x << " " << s.y << endl;
		if (s.type->name == "Entrance") {
			player->x = s.x - player->show_width / 2;
			player->y = s.y - player->show_height;

			int midX = s.x / Block::BLOCKSIZE;
			int midY = s.y / Block::BLOCKSIZE;

			for (int x = 0; x < 2; x++) {
				for (int y = 0; y < 2; y++) {
					blocks.push_back(new Block(midX + x - 1, midY + y - 1, getBlockData("SpawnPlatform" + to_string(x) + to_string(y))));
				}
			}
		}
		else if (s.type->name == "Enemy") {
			entities.push_back(new Enemy(s.x, s.y, enemyDatas[0], window));
		}
	}
}

void World::edgeCleanup(string toEdge) {
	for (int x = 0; x < WORLDSIZE; x++) {
		for (int y = 0; y < WORLDSIZE; y++) {
			Block* b = blocks[index(x, y)];
			// cout << b->type->name.substr(0, toEdge.size()) << endl;
			if (b->type->name == toEdge + "11") {
				int xdelta = 1;
				int ydelta = 1;

				// cout << ">???\n";

				if (blocks[index(x + 1, y)]->type->name.substr(0, toEdge.size()) != toEdge) {
					xdelta = 2;
				}
				if (blocks[index(x - 1, y)]->type->name.substr(0, toEdge.size()) != toEdge) {
					xdelta = 0;
				}
				if (blocks[index(x, y + 1)]->type->name.substr(0, toEdge.size()) != toEdge) {
					ydelta = 2;
				}
				if (blocks[index(x, y - 1)]->type->name.substr(0, toEdge.size()) != toEdge) {
					ydelta = 0;
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

void World::draw(RenderWindow* window, bool front) {
	bool extendXP;
	bool extendXN;
	bool extendYP;
	bool extendYN;

	fixPreLoop(window, extendXP, extendXN, extendYP, extendYN);

	for (Block* b : blocks) {
		if (b->type->front == front) {
			loopPreFix(b, extendXP, extendXN, extendYP, extendYN);
			window->render(b);
			loopPostFix(b);
		}
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

	bool extendXP = object->x > 3 * World::WORLDLENGTH / 4;
	bool extendXN = object->x < 1 * World::WORLDLENGTH / 4;
	bool extendYP = object->y > 3 * World::WORLDLENGTH / 4;
	bool extendYN = object->y < 1 * World::WORLDLENGTH / 4;

	bool collided = true;

	unsigned int rounds = 0;

	for (Block* b : blocks) {
		loopPreFix(b, extendXP, extendXN, extendYP, extendYN);
	}

	while (collided && rounds < 10) {
		collided = false;
		for (Block* b : blocks) {
			//  && blockInRect(b, object->x - Block::BLOCKSIZE, object->y - Block::BLOCKSIZE, object->show_width + Block::BLOCKSIZE * 2, object->show_height + Block::BLOCKSIZE * 2)
			if (!b->type->permissable) {
				// /*
				SDL_Rect bobj = b->getRect();
				const SDL_Rect* bstar = &bobj;

				gobj = object->getRect();
				if (SDL_IntersectRect(gstar, bstar, intersect) == SDL_TRUE) {
					collided = true;
					rounds++;

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
	for (Block* b : blocks) {
		loopPostFix(b);
	}

	return rounds != 0;
}