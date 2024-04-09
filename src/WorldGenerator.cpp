#include "WorldGenerator.hpp"

pair<int, int> flippedIndex(int num, vector<pair<int, int>>& degree45, int subtract) {
	int actualNum = num % degree45.size();
	int falseNum = (num - actualNum) / degree45.size();
	if (falseNum % 2) { // REVERSE
		actualNum = degree45.size() - actualNum - 1;
	}
	int first = degree45[actualNum].first - subtract;
	int second = degree45[actualNum].second;
	if (((falseNum + 1) / 2) % 2) { // SWAP
		swap(first, second);
	}
	first = (((falseNum + 2) / 4) % 2) ? -first : first; // FIRST NEGATIVE
	second = ((falseNum / 4) % 2) ? -second : second; // SECOND NEGATIVE
	return {first, second};
}

vector<pair<int, int>> getDegrees(int r) {
	int xSquared = r * r;
	int makeCloser = (r - 1) * (r - 1);
	int change = 2 * r - 1;
	int x = r;
	int y = 0;
	vector<pair<int, int>> degree45;

	while (y <= x) {
		degree45.push_back({x, y});
		xSquared -= 2 * y + 1;
		if (xSquared <= makeCloser) {
			change -= 2;
			makeCloser -= change;
			x--;
			degree45.push_back({x, y});
		}
		y++;
	}

	return degree45;
}

void generateWorld(World* world, RenderWindow* window, Player* player, vector<GameObject*>& entities, vector<EnemyData*> enemyDatas) {
	// CLEAR PREVIOUS WORLD
	world->structures.clear();
	while (!world->blocks.empty()) {
		delete world->blocks[0];
		world->blocks.erase(world->blocks.begin());
	}
	if (world->shrub != nullptr) {
		world->shrub = nullptr; // Deleted in entities
	}
	delete world->boss;

	// cout << "Blocks: " << world->blocks.size() << endl;

	// LOAD BLOCKS
	world->loadBlocks(window);

	// CREATE BOSS (NOW WITH DIFFICULTY)
	world->boss = new EnemyData("Shrub", 10, 2, -1, -1, 1000, 1, 0.8, 0.2, SHRUB, {
		{"ShrubBattleStart", SpriteSheet(window->loadTexture("res/gfx/ShrubBattleStart.png"), 10, 1, 5)}, 
		{"ShrubAttackL", SpriteSheet(window->loadTexture("res/gfx/ShrubAttackL.png"), 10, 1, 5)}, 
		{"ShrubAttackR", SpriteSheet(window->loadTexture("res/gfx/ShrubAttackR.png"), 10, 1, 5)}, 
		{"ShrubIdleL", SpriteSheet(window->loadTexture("res/gfx/ShrubIdleL.png"), 2, 1, 5)}, 
		{"ShrubIdleR", SpriteSheet(window->loadTexture("res/gfx/ShrubIdleR.png"), 2, 1, 5)}, 
		{"ShrubRunL", SpriteSheet(window->loadTexture("res/gfx/ShrubRunL.png"), 4, 1, 5)}, 
		{"ShrubRunR", SpriteSheet(window->loadTexture("res/gfx/ShrubRunR.png"), 4, 1, 5)}, 
		{"ShrubDeath", SpriteSheet(window->loadTexture("res/gfx/ShrubDeath.png"), 6, 1, 5)}, 
	}, "ShrubBattleStart", 2000, 120, 120, world);

	// GENERATE WORLD
	auto seed = (unsigned) time(NULL);
	// auto seed = 1712205274;
	cout << "SEED: " << seed << endl;

	PerlinNoise pn = PerlinNoise(seed);
	srand(seed);
	random(); // FIRST RAND CALL IS LINEAR TO SEED [BURN IT]
	// double z = random();
	// cout << z << endl;

	// /*

	// GENERATE STRUCTURE LOCATIONS
	if (random() > 0.5) {
		world->structures.push_back(Structure(world->STRUCTYPES[0], (int) ((random() * World::WORLDLENGTH) / Block::BLOCKSIZE) * Block::BLOCKSIZE, 0));
	}
	else {
		world->structures.push_back(Structure(world->STRUCTYPES[0], 0, (int) ((random() * World::WORLDLENGTH) / Block::BLOCKSIZE) * Block::BLOCKSIZE));		
	}
	

	// world->structures.push_back(Structure(world->STRUCTYPES[0], World::WORLDLENGTH / 2, World::WORLDLENGTH / 2 + 2000));

	world->structures.push_back(Structure(world->STRUCTYPES[1], World::WORLDLENGTH / 2, World::WORLDLENGTH / 2));
	// world->structures.push_back(Structure(world->STRUCTYPES[3], World::WORLDLENGTH / 2, World::WORLDLENGTH / 2 + 2000));

	// /*
	int numStructs = World::WORLDSIZE * World::WORLDSIZE / 3000.0 + (1 + random());
	int minChests = World::WORLDSIZE / 100;
	for (int x = 0; x < minChests + numStructs; x++) {
		int x1 = random() * World::WORLDLENGTH;
		int y1 = random() * World::WORLDLENGTH;

		int xdelta = 0;
		int ydelta = 0;

		float r = random();
		StructureType* type = nullptr;

		if (x < minChests) {
			type = world->STRUCTYPES[2];
		}
		else {
			for (StructureType* st : world->STRUCTYPES) {
				if (st->size < 3) {
					// cout << st->name << endl;
					r -= st->rarity;
					// cout << r << endl;
					if (r < 0) {
						// cout << "SELECTED: " << st->name << endl;
						type = st;
						break;
					}
				}
			}
		}

		for (int n = 1; n < 10; n++) {
			for (Structure& s : world->structures) {
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

				float power = type->size * s.type->size * World::SEPERATION_FORCE_FACTOR / sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
				float angle = pointAngleBetween(x1, y1, x2, y2);

				xdelta += power * cos(angle);
				ydelta -= power * sin(angle);

				// cout << "power: " << power << endl;
				// cout << "angle: " << angle * 180 / M_PI << endl;
				// cout << "x1: " << x1 << " y1: " << y1 << endl;
				// cout << "x2: " << x2 << " y2: " << y2 << endl;
				// cout << "s.x: " << s.x << " s.y: " << s.y << endl;
				// cout << "power * cos(angle): " << power * cos(angle) << " -power * sin(angle): " << -power * sin(angle) << endl;
				
			}
			xdelta /= n;
			ydelta /= n;

			x1 += xdelta;
			y1 += ydelta;

			x1 -= int(floor(x1 / World::WORLDLENGTH)) * World::WORLDLENGTH;
			y1 -= int(floor(y1 / World::WORLDLENGTH)) * World::WORLDLENGTH;

			if (xdelta * ydelta < 10) {
				// cout << n << endl;
				break;
			}
		}

		world->structures.push_back(Structure(type, x1, y1));
	}
	

	// float zHeight = random();
	// vector<float> testingTheWaters;
	double biggestValue = 0;
	for (float x = 0; x <= 1; x += 0.5) {
		for (float y = 0; y <= 1; y += 0.5) {
			for (float z = 0; z <= 1; z += 0.5) {
				biggestValue = max(biggestValue, pn.noise(x, y, z));
				// testingTheWaters.push_back(pn.noise(x, y, z));
			}
			// cout << endl;
		}
		// cout << endl;
	}
	// cout << endl;
	biggestValue = (int(10 * biggestValue) + (int(10 * biggestValue) % 2) - 1) / 10.0;
	// cout << biggestValue << endl;

	// PLACE DIRT AND GRASS
	for (int x = 0; x < World::WORLDSIZE; x++) {
		float xangle = M_PI * 2.0 * (x + 1) / (World::WORLDSIZE + 1);
		for (int y = 0; y < World::WORLDSIZE; y++) {
			float yangle = M_PI * 2.0 * (y + 1) / (World::WORLDSIZE + 1);
			// /*
			float val = pn.noise(sin(xangle) / 2 + 0.5, sin(yangle) / 2 + 0.5, cos(xangle) * cos(yangle) / 2 + 0.5);
			if (val > biggestValue && world->notNearStructure(x, y)) {
				// world->blocks.push_back(new Block(x, y, world->getBlockData("DirtPath")));
				world->blocks.push_back(new Block(x, y, world->getBlockData("GrassBlock11")));
			}
			else if (int(val * 100) % 20 < 3) {
				world->blocks.push_back(new Block(x, y, world->getBlockData("DirtPath")));				
			}
			else {
				world->blocks.push_back(new Block(x, y, world->getBlockData("Grass11")));
			}
		}
	}
	
	// SORT BY LOW TO HIGH (Unecessary?)
	// sort(world->blocks.begin(), world->blocks.end(), blockCompare);

	// cout << "SPIN: " << SPIN << endl;
	// PLACE STRUCTURES
	for (Structure& s : world->structures) {
		// cout << s.x << " " << s.y << endl;
		int midX = s.x / Block::BLOCKSIZE;
		int midY = s.y / Block::BLOCKSIZE;

		if (s.type->name == "Entrance") {
			// cout << s.x << " " << s.y << endl;
			player->x = s.x - player->show_width / 2;
			player->y = s.y - player->show_height;

			for (int x = 0; x < 2; x++) {
				for (int y = 0; y < 2; y++) {
					world->blocks.push_back(new Block(midX + x - 1, midY + y - 1, world->getBlockData("SpawnPlatform" + to_string(x) + to_string(y))));
				}
			}
		}
		else if (s.type->name == "Chest") {
			// cout << s.x << " " << s.y << endl;
			int type = random() * 3;
			if (type) {
				world->blocks.push_back(new Block(midX, midY, world->getBlockData("ChestSmallSideClosed"), type - 1));
			}
			else {
				world->blocks.push_back(new Block(midX, midY, world->getBlockData("ChestSmallForwardClosed")));
			}
		}
		else if (s.type->name == "Enemy") {
			// /*
			vector<Enemy*> group;
			EnemyData* ed = enemyDatas[(int) (random() * enemyDatas.size())];
			// EnemyData* ed = enemyDatas[3];
			// cout << "Enemy: " << ed->name << " " << ed->ai << " " << s.x << " " << s.y << endl;
			float enemyMod = world->d.getOption(MOREENEMIES) ? 2 : 1;
			for (int x = 0; x < 1 + enemyMod * (random() + random() + random()); x++) {
				Enemy* enemy = new Enemy(s.x + 5 * Block::BLOCKSIZE * (0.5 - random()), s.y + 5 * Block::BLOCKSIZE * (0.5 - random()), ed, window);
				entities.push_back(enemy);
				group.push_back(enemy);
			}

			for (Enemy* enemy : group) {
				enemy->group = group;
			}
			// */
			
		}
		else if (s.type->name == "Exit") {
			int r = round(7 + 2 * (random() + random() + random()));

			BlockData* fullBrick = world->getBlockData("BrickBlock03");
			BlockData* upRight = world->getBlockData("BrickBlock10");
			BlockData* rightDown = world->getBlockData("BrickBlock11");
			BlockData* rightVertical = world->getBlockData("BrickBlock12");
			BlockData* leftVertical = world->getBlockData("BrickBlock13");

			vector<pair<int, int>> degree45 = getDegrees(r);
			vector<pair<int, int>> pillarCircle = getDegrees(r * 3 / 4);
			int pillarMod = random() * pillarCircle.size();

			pair<int, int> battleCenter = {midX, midY};
			pair<int, int> left = {-1, 0};
			pair<int, int> up = {0, -1};
			pair<int, int> down = {0, 1};
			int sideMod = 0;
			bool nonEdge = false;
			for (int z = 2; z < degree45.size(); z++) {
				// flippedIndex(z + degree45.size(), degree45);
				world->blocks.push_back(new Block(battleCenter + flippedIndex(z + 0 * degree45.size(), degree45), fullBrick));
				world->blocks.push_back(new Block(battleCenter + flippedIndex(z + 1 * degree45.size() - 2, degree45), fullBrick));
				world->blocks.push_back(new Block(battleCenter + flippedIndex(z + 2 * degree45.size(), degree45), fullBrick));
				world->blocks.push_back(new Block(battleCenter + flippedIndex(z + 3 * degree45.size() - 2, degree45), fullBrick));
				world->blocks.push_back(new Block(battleCenter + flippedIndex(z + 4 * degree45.size(), degree45), fullBrick));
				world->blocks.push_back(new Block(battleCenter + flippedIndex(z + 5 * degree45.size() - 2, degree45), fullBrick));
				world->blocks.push_back(new Block(battleCenter + flippedIndex(z + 6 * degree45.size(), degree45), fullBrick));
				world->blocks.push_back(new Block(battleCenter + flippedIndex(z + 7 * degree45.size() - 2, degree45), fullBrick));
			}
			// /*
			for (int z = 0; z < 2; z++) {
				world->blocks.push_back(new Block(battleCenter + flippedIndex(z + 0 * degree45.size(), degree45), fullBrick, BOSS));
				world->blocks.push_back(new Block(battleCenter + flippedIndex(2 * degree45.size() - z - 1, degree45), fullBrick, BOSS));
				world->blocks.push_back(new Block(battleCenter + flippedIndex(z + 2 * degree45.size(), degree45), fullBrick, BOSS));
				world->blocks.push_back(new Block(battleCenter + flippedIndex(4 * degree45.size() - z - 1, degree45), fullBrick, BOSS));
				world->blocks.push_back(new Block(battleCenter + flippedIndex(z + 4 * degree45.size(), degree45), fullBrick, BOSS));
				world->blocks.push_back(new Block(battleCenter + flippedIndex(6 * degree45.size() - z - 1, degree45), fullBrick, BOSS));
				world->blocks.push_back(new Block(battleCenter + flippedIndex(z + 6 * degree45.size(), degree45), fullBrick, BOSS));
				world->blocks.push_back(new Block(battleCenter + flippedIndex(8 * degree45.size() - z - 1, degree45), fullBrick, BOSS));				
			}
			
			for (int x = 0; x < 3; x++) {
				for (int y = 0; y < 3; y++) {
					pair<int, int> pii = {x - 1, r + y - 2};
					world->blocks.push_back(new Block(battleCenter + pii, world->getBlockData("BrickArch" + to_string(x) + to_string(y))));			
				}
			}
			for (int z = 0; z < 8; z++) {
				pair<int, int> pii = battleCenter + flippedIndex(pillarMod + z * pillarCircle.size(), pillarCircle);
				if (random() < 0.5) {
					world->blocks.push_back(new Block(pii, world->getBlockData("ShortPillar")));
				}
				else {
					world->blocks.push_back(new Block(pii, world->getBlockData("TallPillar0")));
					world->blocks.push_back(new Block(pii + up, world->getBlockData("TallPillar1")));
				}
			}

			for (int x = 0; x < 4; x++) {
				for (int y = 0; y < 5; y++) {
					world->blocks.push_back(new Block(midX + x - 2, midY + y - 5, world->getBlockData("BigTree" + to_string(x) + to_string(y))));
				}
			}
		}
	}

	// SIGNS
	vector<SignInfo> vsi = {
		{"SidewaysSign", 3, 1, 0}, 
		{"SidewaysSign", 3, -1, 0},
		{"DownRightSign", 2, -1, -1},
		{"DownLeftSign", 2, 1, -1},
		{"UpRightSign", 2, 1, 1},
		{"UpLeftSign", 2, -1, 1},
	};

	int numSigns = 0;

	while (numSigns < world->structures.size()) {
		for (Structure& s : world->structures) {
			// cout << "s.x: " << s.x << " s.y: " << s.y << endl;
			if (s.type->name == "Chest" || s.type->name == "Exit") {
				for (int x = 0; x < s.type->size + 1; x++) {
					for (SignInfo& si : vsi) {
						float distanceMod = random() * (x + 1) / (s.type->size + 1);

						int distance = World::WORLDSIZE * distanceMod / 2;
						// int distance = 5;
						// cout << "distance: " << distance << endl;
						// int diagDist = sqrt(distance / 2);

						// cout << "si.x: " << si.x << " si.y: " << si.y << endl;
						int midX = s.x / Block::BLOCKSIZE + si.x * distance;
						int midY = s.y / Block::BLOCKSIZE + si.y * distance;

						// cout << "midX: " << midX << " midY: " << midY << endl;

						midX -= int(floor(midX / World::WORLDSIZE)) * World::WORLDSIZE;
						midY -= int(floor(midY / World::WORLDSIZE)) * World::WORLDSIZE;

						if (world->isSafePosition(midX, midY)) {
							// cout << "SIGN " << midX << " " << midY << endl;
							numSigns++;
							for (int y = 0; y < si.size; y++) {
								world->blocks.push_back(new Block(midX, midY + 1 - y, world->getBlockData(si.name + to_string(y))));
							}
						}
					}
				}
			}
		}
	}

	// TREES AND BUSHES
	int numGrowth = (World::WORLDSIZE * World::WORLDSIZE / 20.0) * (1 + random());
	for (int z = 0; z < numGrowth; z++) {
		int midX = random() * World::WORLDSIZE;
		int midY = random() * World::WORLDSIZE;

		if (!world->isSafePosition(midX, midY)) {
			continue;
		}

		if (random() < 0.1) { // TREE
			if (random() < 0.3) {
				for (int x = 0; x < 4; x++) {
					for (int y = 0; y < 5; y++) {
						world->blocks.push_back(new Block(midX + x - 2, midY + y - 2, world->getBlockData("BigTree" + to_string(x) + to_string(y))));
					}
				}
			}
			else {
				for (int x = 0; x < 3; x++) {
					for (int y = 0; y < 3; y++) {
						world->blocks.push_back(new Block(midX + x - 1, midY + y - 1, world->getBlockData("SmallTree" + to_string(x) + to_string(y))));
					}
				}
			}
		}
		else { // BUSH
			if (random() < 0.3) {
				for (int x = 0; x < 2; x++) {
					for (int y = 0; y < 2; y++) {
						world->blocks.push_back(new Block(midX + x - 1, midY + y - 1, world->getBlockData("BigBush" + to_string(x) + to_string(y))));
					}
				}
			}
			else {
				world->blocks.push_back(new Block(midX, midY, world->getBlockData("SmallBush")));
			}			
		}
	}

	// DIRT EDGE CLEANUP
	world->tempEdgeCleanup("Grass", "DirtPath");
	world->tempEdgeCleanup("GrassBlock", "Grass11");
	world->edgeCleanup("Grass");
	world->edgeCleanup("GrassBlock");

	// GRASS DIRT RANDO
	for (int x = 0; x < World::WORLDSIZE; x++) {
		for (int y = 0; y < World::WORLDSIZE; y++) {
			if (world->blocks[index(x, y)]->type->name == "Grass11" && random() > 0.6) {
				world->blocks[index(x, y)]->switchBlockType(world->getBlockData("GrassRandom" + to_string(int(3 * random()))));
			}
			else if (world->blocks[index(x, y)]->type->name == "DirtPath" && random() > 0.6) {
				world->blocks[index(x, y)]->switchBlockType(world->getBlockData("DirtRandom" + to_string(int(2 * random()))));
			}
		}
	}
	// */

	// cout << "WorldGen should be player: " << entities[0] << endl;
}

// Maybe if i dont wanna kill myself ill implement this but i actually care for my mental health fuck this fuck this fuck this fuck this fuck this
/*
				if ((degree45[z] - degree45[z + 1]).first == 1) { // BEFORE SHIFT
					// /*
					if (nonEdge) {
						world->blocks.push_back(new Block(battleCenter + flippedIndex(z + 0 * degree45.size(), degree45), fullBrick));
					}
					else {
						world->blocks.push_back(new Block(battleCenter + flippedIndex(z + 0 * degree45.size(), degree45), world->getBlockData("BrickBlock0" + to_string(sideMod))));
						sideMod = (sideMod + 1) % 3;
					}
					world->blocks.push_back(new Block(battleCenter + flippedIndex(z + 1 * degree45.size(), degree45), fullBrick));

				}
				else if ((degree45[z] - degree45[z - 1]).second == 1) { // NO SHIFT
					// /*
					if (nonEdge) {
						world->blocks.push_back(new Block(battleCenter + flippedIndex(z + 0 * degree45.size(), degree45), fullBrick));
						world->blocks.push_back(new Block(battleCenter + flippedIndex(z + 0 * degree45.size(), degree45) + left, rightVertical));
					}
					else {
						world->blocks.push_back(new Block(battleCenter + flippedIndex(z + 0 * degree45.size(), degree45), fullBrick));
						world->blocks.push_back(new Block(battleCenter + flippedIndex(z + 0 * degree45.size(), degree45), rightVertical));
					}
					world->blocks.push_back(new Block(battleCenter + flippedIndex(z + 1 * degree45.size(), degree45), world->getBlockData("BrickBlock0" + to_string(sideMod))));
					sideMod = (sideMod + 1) % 3;
				}
				else { // DURING SHIFT
					nonEdge = true;
					// /*
					world->blocks.push_back(new Block(battleCenter + flippedIndex(z + 0 * degree45.size(), degree45), world->getBlockData("BrickBlock0" + to_string(sideMod))));
					sideMod = (sideMod + 1) % 3;
					world->blocks.push_back(new Block(battleCenter + flippedIndex(z + 0 * degree45.size(), degree45) + left, rightVertical));
					world->blocks.push_back(new Block(battleCenter + flippedIndex(z + 1 * degree45.size(), degree45), world->getBlockData("BrickBlock0" + to_string(sideMod))));
					sideMod = (sideMod + 1) % 3;
					world->blocks.push_back(new Block(battleCenter + flippedIndex(z + 1 * degree45.size(), degree45) + up, rightVertical));
				}
*/