#include <SDL2/SDL_mixer.h>

#include <chrono>

#include "PerlinNoise.hpp"
#include "Button.hpp"
#include "Player.hpp"
#include "World.hpp"
#include "WorldGenerator.hpp"
#include "Enemy.hpp"
#include "EnemyData.hpp"
#include "utils.hpp"
#include "ItemDrop.hpp"

using namespace std;

/*
REMOVED
Decreased Player movement acceleration (- speed + traction) [max speed = speed * traction / (1 - traction)], attack speed, less slots, Weapons take double disrepair upon use, and smaller pickup range

1 - Enemies inflict more statuses and for longer
2 - Enemies can change their home location
4 - Improved enemy AI
8 - Increased projectile speed
16 - Some enemies can Block, Spotdodge, Roll and/or Reflect
32 - Increased enemy hp, defence, movement speed, attack speed, damage, knockback resistance
64 - Enemies and Bosses get new moves
128 - Enemies respawn
256 - You don't slowly regenerate health

TEMP FOR THE CURRENT VERSION

1 - Enemy damage
2 - Enemy knockback resistance (x2 + 0.1)
4 - Enemy speed
8 - Weapon no pierce-u (unless spear)
16 - Bonus Items
32 - Max HP x2
64 - Enemy HP x2
128 - Remove regen health
*/

bool height(GameObject* go1, GameObject* go2) {
	return go1->y < go2->y;
}

void backToMainMenu(vector<void*> passingArgument) {
	SCREEN* s = (SCREEN*) passingArgument[0];
	Button* b = (Button*) passingArgument[1];
	*s = MAIN;
	b->show = false;
}

void diffSelect(vector<void*> passingArgument) {
	SCREEN* s = (SCREEN*) passingArgument[0];
	Button* b = (Button*) passingArgument[1];
	*s = DIFFICULTYSELECT;
	b->show = false;
}

void startGame(vector<void*> passingArgument) {
	// cout << "startGame\n";

	SCREEN* s = (SCREEN*) passingArgument[0];
	Button* b = (Button*) passingArgument[1];
	World* world = (World*) passingArgument[2];
	vector<GameObject*>* entities = (vector<GameObject*>*) passingArgument[3];
	RenderWindow* window = (RenderWindow*) passingArgument[4];
	Player* player = (Player*) passingArgument[5];
	vector<EnemyData*>* enemyDatas = (vector<EnemyData*>*) passingArgument[6];
	float* difficulty = (float*) passingArgument[7];
	vector<ItemData*>* itemDatas = (vector<ItemData*>*) passingArgument[8];

	// cout << "player: " << player << endl;
	// cout << "world: " << world << endl;
	// cout << "FUNCprev: " << player->actualMaxHp << endl;

	// cout << "preDiff\n";
	// /*
	world->d = Difficulty(*difficulty);

	while (!enemyDatas->empty()) {
		// cout << "enemyDatas\n";
		delete enemyDatas->at(0);
		enemyDatas->erase(enemyDatas->begin());
	}

	// cout << entities->at(0) << endl;
	while (!entities->empty()) {
		// cout << "entities: " << entities->at(0) << endl;
		if (entities->at(0) != player) {
			delete entities->at(0);
		}
		entities->erase(entities->begin());
	}	
	entities->push_back(player);

	// cout << "createEnemyDatea\n";
	*enemyDatas = {
		new EnemyData("Flowy", 20, 40, 500, 60, 100, 0, 0.95, 0, BOUNCING, {
			{"Bounce", SpriteSheet(window->loadTexture("res/gfx/Flowy.png"), 1, 1, 1)}, 
			{"Idle", SpriteSheet(window->loadTexture("res/gfx/Flowy.png"), 1, 1, 1)}
		}, "Idle", 2000, 68, 90, world),
		new EnemyData("Mossling", 15, 40, -1, -1, 70, 0, 0.2, 0, SNEAKING, {
			{"Hidden", SpriteSheet(window->loadTexture("res/gfx/grasstiles.png"), 18, 9, 1)}, 
			{"Walking", SpriteSheet(window->loadTexture("res/gfx/Mossling_Spritesheet.png"), 4, 7, 5)}
//			{"Walking", SpriteSheet(window->loadTexture("res/gfx/Mossling_Spritesheet.png"), 1, 1, 1)}
		}, "Walking", 500, 80, 80, world),
		new EnemyData("Acorn", 5, 1, 60, -1, 40, 2, 0.95, 0.05, SPIN, {
			{"Spin", SpriteSheet(window->loadTexture("res/gfx/acorntop.png"), 2, 2, 1)}, 
		}, "Spin", 2000, 80, 80, world),
		new EnemyData("Cacty", 10, 40, 60, 60, 80, 0, 0.9, 0, BOUNCING, {
			{"Bounce", SpriteSheet(window->loadTexture("res/gfx/Cacty.png"), 2, 2, 1)}, 
			{"Idle", SpriteSheet(window->loadTexture("res/gfx/Cacty.png"), 2, 2, 1)}, 
		}, "Idle", 1000, 80, 80, world),
	};

	// cout << "modEnemyData\n";
	/*
	for (EnemyData* ed : *enemyDatas) {
		if (!world->d.getOption(ENEMYDAMAGE)) {
			ed->damage /= 1.5;
			ed->damage++;
		}
		if (!world->d.getOption(ENEMYSPEED)) {
			ed->movementspeed /= 1.5;
			ed->movementspeed++;
		}
		else {
			ed->attackFrames *= 0.75;
			ed->attackdelay *= 0.75;
		}
		if (world->d.getOption(ENEMYKNOCKBACK)) {
			ed->knockbackResistance = 1.5 * ed->knockbackResistance + 0.1;
		}
		if (world->d.getOption(ENEMYHP)) {
			ed->maxHP *= 1.5;
			ed->defence *= 1.5;
			ed->defence += 1;
		}
	}
	// */

	// cout << "player: " << player << endl;
	// cout << "world: " << world << endl;
	// cout << "readyToPlay\n";
	// cout << "FUNCprev: " << player->actualMaxHp << endl;
	// player->slotTexture = window->loadTexture("res/gfx/slot.png");
	// player->selectedSlotTexture = window->loadTexture("res/gfx/selectedSlot.png");
	player->readyToPlay(world);
	// /*
	// cout << "item!\n";
	player->giveItem(new Item(itemDatas->at(4)));
	if (!world->d.getOption(BONUSITEMSLOTS)) {
		// cout << "item?\n";
		player->giveItem(new Item(itemDatas->at(int(random() * itemDatas->size()))));
	}
	// */
	// cout << "generateWorld\n";
	generateWorld(world, window, player, *entities, *enemyDatas);
	b->show = false;

	*s = GAME;
}

void runGame() {
	const int FPS = 60;

	srand((unsigned) time(NULL));
	random(); // Burn the witch (first one lol)

	if (SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0) {
		// cout << "SDL could not initialize! SDL Error: " << SDL_GetError() << "\n";
	}
	if (!IMG_Init(IMG_INIT_PNG)) {
		// cout << "IMG PNG Failure: " << SDL_GetError() << "\n";
	}
	if(TTF_Init() == -1) {
	    cout << "TTF_Init: " << TTF_GetError() << endl;
	}

	//Initialize SDL_mixer
	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 8, 2048) < 0) {
		// cout << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << "\n";
	}

	vector<string> splashTexts = {
		"100% Naturally Grown!", 
		"Non-GMO", 
		"Support Your Local Developers!", 
		"Sea of Rogues: Nature Edition", 
		"The joke is that we actually made a game... get it?"
	};

 	string nameAddon = splashTexts[int(random() * splashTexts.size())];

	map<string, Mix_Chunk*> soundBoard = {
	};
	map<string, Mix_Music*> OST = {
		{"Boss", Mix_LoadMUS("res/aud/naturogueboss.wav")}, 
		{"Level", Mix_LoadMUS("res/aud/naturoguelevel.wav")}, 
	};

	Mix_Music* backgroundMusic = OST["Level"];

	bool gameRunning = true;
	bool playingMusic = true;
	SDL_Event event;

	RenderWindow window(("Naturogue: " + nameAddon).c_str());

	shared_ptr<SDL_Texture> slotTexture(window.loadTexture("res/gfx/slot.png"), sdl_deleter());
	shared_ptr<SDL_Texture> selectedSlotTexture(window.loadTexture("res/gfx/selectedSlot.png"), sdl_deleter());
	shared_ptr<SDL_Texture> weapons1(window.loadTexture("res/gfx/Tools_weapons.png"), sdl_deleter());
	shared_ptr<SDL_Texture> knife(window.loadTexture("res/gfx/knife.png"), sdl_deleter());

	Player* player = new Player(&window, slotTexture, selectedSlotTexture);

	// cout << slotTexture.get() << endl;
	// cout << selectedSlotTexture.get() << endl;

	vector<Button*> buttons;
	vector<GameObject*> entities;

	vector<EnemyData*> enemyDatas;
	/*
		new EnemyData("Mossling", 5, 40, 500, 60, 100, 0, 0.2, 0, SNEAKING, {
			{"Hidden", SpriteSheet(window.loadTexture("res/gfx/grasstiles.png"), 18, 9, 1)}, 
			{"Walking", SpriteSheet(window.loadTexture("res/gfx/Flowy.png"), 1, 1, 1)}
		}, "Walking", 1000),
	*/
	vector<Vector2f> boomerangHitbox = {{15, 2}, {2, 15}, {2, 2}};
	vector<Vector2f> swordHitbox = {{7, 10}, {7, 8}, {16, 0}, {16, 2}};
	vector<Vector2f> spearHitbox = {{2, 16}, {0, 14}, {11, 1}, {15, 0}, {15, 5}};

	vector<ItemData*> itemDatas = {
		new ItemData(knife, 480, 480, 0, 0, 80, 80, "Knife", 3 * M_PI / 2,                  M_PI / 4, 30, 40, 4, false, false, 2.5, 0.95, 2, {}),
		new ItemData(weapons1, 16, 16, 0, 0, 80, 80, "Dangerang", 5 * M_PI / 4,             M_PI / 4, 15, 45, 8, true, false, 3.5, 0.95, 10, {BOOMERANG, DANGER}, boomerangHitbox),
		new ItemData(weapons1, 16, 16, 1, 0, 80, 80, "Bloomerang", 5 * M_PI / 4,            M_PI / 2, 10, 30, 8, true, false, 3, 0.95, 5, {BOOMERANG}, boomerangHitbox),
		new ItemData(weapons1, 16, 16, 2, 0, 120, 120, "Qhasm's Tippy", 5 * M_PI / 4,       M_PI / 2, 35, 20, 6, false, false, 2, 0.9, 7, {TIPPER}, swordHitbox),
		new ItemData(weapons1, 16, 16, 3, 0, 70, 70, "Stick", 5 * M_PI / 4,                 M_PI / 5, 30, 30, 2, true, false, 2.5, 0.8, 5, {}, swordHitbox),
		new ItemData(weapons1, 16, 16, 4, 0, 80, 80, "Bat", 5 * M_PI / 4,                   M_PI / 2, 60, 20, 10, true, false, 1.5, 0.95, 20, {}, swordHitbox),
		new ItemData(weapons1, 16, 16, 5, 0, 80, 80, "Rock", 5 * M_PI / 4,                  M_PI / 2, 15, 15, 20, true, false, 2, 0.95, 4, {}), 
		new ItemData(weapons1, 16, 16, 7, 0, 80, 80, "KB", 5 * M_PI / 4,                    M_PI / 2, 20, 20, 20, true, false, 2.5, 0.97, 30, {SPEAR}, spearHitbox),
		new ItemData(weapons1, 16, 16, 8, 0, 80, 80, "Spear", 5 * M_PI / 4,                 M_PI / 2, 30, 20, 20, false, true, 5, 0.85, 3, {SPEAR}, spearHitbox),
		new ItemData(weapons1, 16, 16, 10, 0, 80, 80, "Handyman's Hammer", 3 * M_PI / 2,    M_PI, 15, 25, 20, true, false, 2, 0.85, 4, {HAMMER}),
		new ItemData(weapons1, 16, 16, 11, 0, 80, 80, "Heavy Hammer", 3 * M_PI / 2,         M_PI, 20, 30, 20, false, false, 3, 0.95, 6, {HAMMER}),
		new ItemData(weapons1, 16, 16, 12, 0, 80, 80, "Swinging Hammer", 5 * M_PI / 4,      6 * M_PI, 30, 15, 60, true, false, 2, 0.9, 10, {HAMMER}),
		new ItemData(weapons1, 16, 32, 13, 0, 80, 160, "Hamber", 3 * M_PI / 2,              M_PI / 2, 0, 0, 30, true, false, 1, 0.95, 40, {HAMMER}),
	};

	// player->items[0].holding = new Item(itemDatas[8]);
	// player->items[1].holding = new Item(itemDatas[3]);
	// player->items[2].holding = new Item(itemDatas[13]);

	entities.push_back(player);
	// cout << "a1\n";
	// entities.push_back(new Enemy(-100, -100, enemyDatas[0]));
	// cout << "b1\n";

	World* world = new World(&window, player);
	window.world = world;
	// player->readyToPlay(world);
	// cout << "EDNDDNDNDNDN!\n";
	// player->readyToPlay(world);

	window.zoom = 0.75;

	SCREEN current = MAIN;
	Entity* mainMenu = new Entity(0, 0, window.loadTexture("res/gfx/Naturogue.png"), RenderWindow::WIDTH, RenderWindow::HEIGHT);
	Entity* endCred = new Entity(0, 0, window.loadTexture("res/gfx/endcred.png"), RenderWindow::WIDTH, RenderWindow::HEIGHT);
	buttons.push_back(new Button(&window, "Start", 490, 310, 300, 100, 0, 255, 0, 0, 0, 0, true, &diffSelect));
	buttons.push_back(new Button(&window, "Play", 500, 500, 300, 100, 0, 255, 0, 0, 0, 0, false, &startGame));
	buttons.push_back(new Button(&window, "Retry?", 500, 500, 300, 100, 100, 100, 100, 255, 255, 255, false, &startGame));
	buttons.push_back(new Button(&window, "Main Menu", 500, 300, 300, 100, 100, 100, 100, 255, 255, 255, false, &backToMainMenu));

	bool difficultySlider = false;
	Bar* difficulty = new Bar(&window, RenderWindow::WIDTH, 100, 10, Difficulty::MAX_DIFFICULTY, true);
	difficulty->x = 0;
	difficulty->y = 310;
	difficulty->value = 100;

	while (gameRunning) {
		auto start = chrono::steady_clock().now();

		window.clear();

		if (playingMusic) {
			if (Mix_PlayingMusic() == 0) {
				// REMOVE TO ENABLE MUSIC
				Mix_PlayMusic(backgroundMusic, -1);
			}
		}
		else {
			Mix_HaltMusic();
		}

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				gameRunning = false;
			}
			if (event.type == SDL_MOUSEBUTTONDOWN) {
				SDL_Point p = {event.motion.x, event.motion.y};
				SDL_Rect r = difficulty->getRect();

				for (Button* b : buttons) {
					if (b->show) {
						if (SDL_PointInRect(&p, &(b->area)) == SDL_TRUE) {
							b->clicked = !b->clicked;
						}
					}
				}

				if (current == GAME && !player->animation && player->items[player->selectedSlot].holding != nullptr) {
					player->mousedown = true;
				}

				if (current == DIFFICULTYSELECT && SDL_PointInRect(&p, &r)) {
					difficultySlider = true;
				}
			}
			else if (event.type == SDL_MOUSEBUTTONUP) {
				difficultySlider = false;
				// TODO: YO WONT WORK FULLSCREEN (IT DOES BUT LIKE WHYYYYY??????) ALSO WHY X Y SWAP???
				player->attackAngle = pointAngleBetween(RenderWindow::HEIGHT / 2, RenderWindow::WIDTH / 2, event.motion.y, event.motion.x);
				// cout << "emx: " << event.motion.x << " emy: " << event.motion.y << endl;
				// cout << "rw2: " << RenderWindow::WIDTH / 2 << " rh2: " << RenderWindow::HEIGHT / 2 << endl;
				// cout << "paa: " << player->attackAngle << endl;
				if (event.button.button == SDL_BUTTON_LEFT) {
					// cout << "SWITHGN\n";
					player->swing = true;
					player->mousedown = false;
				}
				if (event.button.button == SDL_BUTTON_RIGHT) {
					player->yeet = true;
					player->mousedown = false;
				}
			}
			else if (event.type == SDL_KEYUP) {
				SDL_Keycode kc = event.key.keysym.sym;
				if (kc == SDLK_RALT) {
					auto flag = SDL_GetWindowFlags(window.window);
					auto is_fullScreen  = flag&SDL_WINDOW_FULLSCREEN;
					SDL_SetWindowFullscreen(window.window, is_fullScreen != SDL_WINDOW_FULLSCREEN);
				}
				// TODO: DEBUG MODE REMOVE IN FINAL VERSION
				/*
				if (kc == SDLK_k) {
					entities.push_back(new ItemDrop(player->x, player->y, new Item(itemDatas[int(random() * itemDatas.size())]), 0));
				}
				if (kc == SDLK_l) {
					player->HP = 0;
				}
				if (kc == SDLK_j) {
					player->x = World::WORLDLENGTH / 2;
					player->y = World::WORLDLENGTH / 2 - 2000;
				}
				// */
			}
			else if (event.type == SDL_KEYDOWN) {
				int num = int(event.key.keysym.sym) - 49; // 48 is 0 key
				if (current == GAME && num > -1 && num < player->slots && !player->animation) {
					player->select(num);
				}
			}
		}

		for (Button* b : buttons) {
			b->show = false;
		}

		if (current == MAIN) {
			buttons[0]->show = true;
			window.render(mainMenu, true);
			window.drawNicelyScaledText(nameAddon, 255, 255, 0, 255, RenderWindow::WIDTH / 4, 200, RenderWindow::WIDTH / 2, 100);
		}
		else if (current == DIFFICULTYSELECT) {
			buttons[1]->show = true;
			window.render(mainMenu, true);

			difficulty->draw(&window, world, entities);
			window.drawNicelyScaledText(to_string(int(difficulty->value)), 0, 0, 0, 255, 600, 200, 100, 100);

			if (difficultySlider) {
				// /*
				int mouseX;
				int mouseY;

				SDL_GetMouseState(&mouseX, &mouseY);

				int value = round(Difficulty::MAX_DIFFICULTY * (mouseX - difficulty->x) / difficulty->show_width);
				if (value < 0) {
					difficulty->value = 0;
				}
				else if (value > Difficulty::MAX_DIFFICULTY) {
					difficulty->value = Difficulty::MAX_DIFFICULTY;
				}
				else {
					difficulty->value = value;
				}
				// */
			}
		}
		else if (current == GAMEOVER) {
			window.drawNicelyScaledText("GAME OVER", 255, 0, 0, 255, 300, 0, 600, 200);

			buttons[2]->show = true;
			buttons[3]->show = true;
		}
		else if (current == WINCREDITS) {
			buttons[3]->show = true;

			window.render(endCred, true);

			window.drawNicelyScaledText("CONGRATULATIONS!", 255, 255, 255, 255, 0, 0, RenderWindow::WIDTH, 100);
			window.drawNicelyScaledText("Difficulty: " + to_string(int(difficulty->value)), 255, 255, 255, 255, 440, 100, 400, 50);
		}		
		else if (current == GAME) {
			// /*
			arrowChange(&window, window.cc.up, &player->input.up, nullptr, {});
			arrowChange(&window, window.cc.left, &player->input.left, nullptr, {});
			arrowChange(&window, window.cc.right, &player->input.right, nullptr, {});
			arrowChange(&window, window.cc.down, &player->input.down, nullptr, {});
			arrowChange(&window, window.cc.okay, &player->input.okay, interact, {world, player, &itemDatas, &entities});

			// cout << "BEFORE: " << player->x << " " << player->y << endl;

			bool extendXP;
			bool extendXN;
			bool extendYP;
			bool extendYN;

			world->draw(&window, entities, false);

			// cout << "Entities may be broken\n";
			fixPreLoop(&window, extendXP, extendXN, extendYP, extendYN);
			for (GameObject* go : entities) {
				loopPreFix(go, extendXP, extendXN, extendYP, extendYN);
			}
			for (Block* b : world->blocks) {
				loopPreFix(b, extendXP, extendXN, extendYP, extendYN);
			}

			// cout << "preSort\n";
			sort(entities.begin(), entities.end(), height);
			// */

			// cout << "Main loop portion\n";
			for (int g = 0; g < entities.size(); g++) {
				GameObject* go = entities.at(g);
				// cout << "g: " << g << " go: " << go << endl;
				bool alive = go->draw(&window, world, entities);
				if (alive) {
					// cout << "deleting and erasing\n";
					delete go;
					entities.erase(entities.begin() + g);
					g--;
				}
			}

			// /*
			for (GameObject* go : entities) {
				loopPostFix(go);
				// cout << "x: " << go->x << " y: " << go->y << endl;
			}
			// cout << "AFTER: " << player->x << " " << player->y << endl;

			world->draw(&window, entities, true);

			for (Block* b : world->blocks) {
				loopPostFix(b);
			}

			for (Slot& s : player->items) {
				s.draw(&window);
			}
			// */

			// CAMERA
			window.x = player->x - ((RenderWindow::WIDTH - player->show_width) / 2) / window.zoom;
			window.y = player->y - ((RenderWindow::HEIGHT - player->show_height) / 2) / window.zoom;

			if (world->shrub != nullptr) {
				world->shrub->healthBar->x = (RenderWindow::WIDTH - world->shrub->healthBar->show_width) / 2;
				world->shrub->healthBar->y = world->shrub->healthBar->show_height / 2;
				if (world->shrub->active) {
					world->shrub->healthBar->value = world->shrub->rageMeter * world->shrub->health / 100;
				}
				else {
					world->shrub->healthBar->value = world->shrub->health;				
				}
				world->shrub->healthBar->draw(&window, world, entities);

				if (backgroundMusic == OST["Level"]) {
					Mix_HaltMusic();
					backgroundMusic = OST["Boss"];
				}
			}
			else if (backgroundMusic == OST["Boss"]) {
				Mix_HaltMusic();
				backgroundMusic = OST["Level"];
				current = WINCREDITS;
			}

			player->healthBar->draw(&window, world, entities);
			if (player->HP <= 0) {
				current = GAMEOVER;
			}
			// cout << "MainGame loop should be player: " << entities.at(0) << endl;
			// cout << "wincreds\n";
			// current = WINCREDITS;
		}

		if (backgroundMusic == OST["Boss"] && current != GAME) {
			Mix_HaltMusic();
			backgroundMusic = OST["Level"];
		}

		// cout << player << endl;
		// cout << "EXTprev: " << player->actualMaxHp << endl;
		for (Button* b : buttons) {
			// cout << b << " " << player->actualMaxHp << endl;
			b->draw(&window, {&current, b, world, &entities, &window, player, &enemyDatas, &difficulty->value, &itemDatas});
		}
		// */

		auto end = chrono::steady_clock().now();
		chrono::duration<double> frameDone = end - start;

		window.display();
		double delay = 1000 * ((1.0 / FPS) - frameDone.count());
		// cout << delay << " " << world->renderSize << endl;
		if (current == GAME) {
			if (delay > 3.5) {
				world->renderSize += 100;
			}
			else {
				world->renderSize -= 100;
			}
			if (world->renderSize > World::WORLDLENGTH * sqrt(2)) {
				world->renderSize = World::WORLDLENGTH * sqrt(2);
			}
			if (world->renderSize < 2 * RenderWindow::SCREENRADIUS / window.zoom) {
				world->renderSize = 2 * RenderWindow::SCREENRADIUS / window.zoom;
			}
		}

		if (delay > 0) {
			SDL_Delay(delay);
		}
	}
}

int main(int argc, char *args[]) {
	runGame();
/*
	PerlinNoise pn;
	for (float x = 0; x < 1; x += 0.25) {
		for (float y = 0; y < 1; y += 0.25) {
			for (float z = 0; z < 1; z += 0.25) {	
				// cout << "1x: " << pn.noise(x, y, z) << endl;
				// cout << "-1x: " << pn.noise(-x, -y, -z) << endl;
			}
		}		
	}
// */
	return 0;
}