#include <SDL2/SDL_mixer.h>

#include <chrono>

#include "PerlinNoise.hpp"
#include "Button.hpp"
#include "Player.hpp"
#include "World.hpp"
#include "Enemy.hpp"
#include "EnemyData.hpp"
#include "utils.hpp"

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
*/

void runGame() {
	const int FPS = 60;

	srand((unsigned) time(NULL));
	if (SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0) {
		cout << "SDL could not initialize! SDL Error: " << SDL_GetError() << "\n";
	}
	if (!IMG_Init(IMG_INIT_PNG)) {
		cout << "IMG PNG Failure: " << SDL_GetError() << "\n";
	}
	if(TTF_Init() == -1) {
	    cout << "TTF_Init: " << TTF_GetError() << endl;
	}

	//Initialize SDL_mixer
	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 8, 2048) < 0) {
		cout << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << "\n";
	}

	map<string, Mix_Chunk*> soundBoard = {
	};
	map<string, Mix_Music*> OST = {
		{"Oppression", Mix_LoadMUS("res/aud/gamers_are_oppressed.wav")}, 
		{"Chill_1", Mix_LoadMUS("res/aud/very_lazy_non_battle_version.wav")}, 
	};

	Mix_Music* backgroundMusic = OST["Chill_1"];

	bool gameRunning = true;
	bool playingMusic = true;
	SDL_Event event;

	RenderWindow window("Naturogue");
	World* world = new World(&window);
	window.world = world;

	shared_ptr<SDL_Texture> slotTexture(window.loadTexture("res/gfx/slot.png"), sdl_deleter());
	shared_ptr<SDL_Texture> selectedSlotTexture(window.loadTexture("res/gfx/selectedSlot.png"), sdl_deleter());
	shared_ptr<SDL_Texture> weapons1(window.loadTexture("res/gfx/Tools_weapons.png"), sdl_deleter());
	shared_ptr<SDL_Texture> knife(window.loadTexture("res/gfx/knife.png"), sdl_deleter());

	Player* player = new Player(&window, slotTexture, selectedSlotTexture);

	window.zoom = 0.75;

	vector<Button*> buttons;
	vector<GameObject*> entities;

	vector<EnemyData*> enemyDatas = {
		new EnemyData("Flowy", 5, 40, 500, 60, 30, 0, 0.9, 0, BOUNCING, {
			{"Bounce", SpriteSheet(window.loadTexture("res/gfx/Flowy.png"), 1, 1, 1)}, 
			{"Idle", SpriteSheet(window.loadTexture("res/gfx/Flowy.png"), 1, 1, 1)}
		}, "Idle")
	};

	vector<ItemData*> itemDatas = {
		new ItemData(knife, 480, 480, 0, 0, 80, 80, "Knife", 3 * M_PI / 2,                  M_PI / 4, 20, 50, 10, false, false),
		new ItemData(weapons1, 16, 16, 0, 0, 80, 80, "Dangerang", 5 * M_PI / 4,             M_PI / 4, 10, 100, 20, true, false),
		new ItemData(weapons1, 16, 16, 1, 0, 80, 80, "Bloomerang", 5 * M_PI / 4,            M_PI / 2, 10, 40, 20, true, false),
		new ItemData(weapons1, 16, 16, 2, 0, 120, 120, "Qhasm's Tippy", 5 * M_PI / 4,       M_PI / 2, 30, 30, 15, false, false),
		new ItemData(weapons1, 16, 16, 3, 0, 40, 40, "Stick", 5 * M_PI / 4,                 M_PI / 5, 5, 5, 5, true, false),
		new ItemData(weapons1, 16, 16, 4, 0, 80, 80, "Bat", 5 * M_PI / 4,                   M_PI / 2, 10, 100, 20, true, false),
		new ItemData(weapons1, 16, 16, 5, 0, 80, 80, "Rock", 5 * M_PI / 4,                  M_PI / 2, 10, 100, 20, true, false), 
		new ItemData(weapons1, 16, 16, 6, 0, 80, 80, "Trickshot", 5 * M_PI / 4,             M_PI / 2, 10, 100, 20, true, false),
		new ItemData(weapons1, 16, 16, 7, 0, 80, 80, "KB", 5 * M_PI / 4,                    M_PI / 2, 10, 100, 20, true, false),
		new ItemData(weapons1, 16, 16, 8, 0, 80, 80, "Spear", 5 * M_PI / 4,                 M_PI / 2, 10, 100, 20, false, true),
		new ItemData(weapons1, 16, 16, 9, 0, 80, 80, "Blossom", 5 * M_PI / 4,               M_PI / 2, 10, 100, 20, false, false),
		new ItemData(weapons1, 16, 16, 10, 0, 80, 80, "Handyman's Hammer", 3 * M_PI / 2,    M_PI / 2, 10, 100, 20, true, false),
		new ItemData(weapons1, 16, 16, 11, 0, 80, 80, "Lightning Man Hammer", 5 * M_PI / 4, M_PI / 2, 10, 100, 20, false, false),
		new ItemData(weapons1, 16, 16, 12, 0, 80, 80, "Swinging Hammer", 5 * M_PI / 4,      M_PI / 2, 10, 100, 30, true, false),
		new ItemData(weapons1, 16, 32, 13, 0, 80, 160, "Hamber", 3 * M_PI / 2,              M_PI / 2, 0, 0, 30, true, false),
	};

	player->items[0].holding = new Item(itemDatas[0]);

	entities.push_back(player);
	// cout << "a1\n";
	entities.push_back(new Enemy(-100, -100, enemyDatas[0]));
	// cout << "b1\n";

	while (gameRunning) {
		auto start = chrono::steady_clock().now();

		window.clear();

		if (playingMusic) {
			if (Mix_PlayingMusic() == 0) {
				// REMOVE TO ENABLE MUSIC
				// Mix_PlayMusic(backgroundMusic, -1);
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
				for (Button* b : buttons) {
					if (b->show) {
						if (SDL_PointInRect(&p, &(b->area)) == SDL_TRUE) {
							b->clicked = !b->clicked;
						}
					}
				}

				if (!player->animation) {
					player->mousedown = true;
				}
			}
			else if (event.type == SDL_MOUSEBUTTONUP) {
				player->attackAngle = angleBetween(RenderWindow::HEIGHT / 2, RenderWindow::WIDTH / 2, event.motion.y, event.motion.x);
				cout << "emx: " << event.motion.x << " emy: " << event.motion.y << endl;
				cout << "rw2: " << RenderWindow::WIDTH / 2 << " rh2: " << RenderWindow::HEIGHT / 2 << endl;
				cout << "paa: " << player->attackAngle << endl;
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
					auto is_fullscreen  = flag&SDL_WINDOW_FULLSCREEN;
					SDL_SetWindowFullscreen(window.window, is_fullscreen != SDL_WINDOW_FULLSCREEN);
				}
			}
			else if (event.type == SDL_KEYDOWN) {
				int num = int(event.key.keysym.sym) - 49; // 48 is 0 key
				if (num > -1 && num < player->slots) {
					player->select(num);
				}
			}

			arrowChange(&window, window.cc.up, &player->input.up, nullptr, {});
			arrowChange(&window, window.cc.left, &player->input.left, nullptr, {});
			arrowChange(&window, window.cc.right, &player->input.right, nullptr, {});
			arrowChange(&window, window.cc.down, &player->input.down, nullptr, {});
		}

		// cout << "BEFORE: " << player->x << " " << player->y << endl;

		world->draw(&window);
		for (GameObject* go : entities) {
			go->draw(&window, world, entities);
		}

		// cout << "AFTER: " << player->x << " " << player->y << endl;

		for (Button* b : buttons) {
			if (b->show) {
				window.render(b);
			}
		}

		// CAMERA
		window.x = player->x - ((RenderWindow::WIDTH - player->show_width) / 2) / window.zoom;
		window.y = player->y - ((RenderWindow::HEIGHT - player->show_height) / 2) / window.zoom;

		auto end = chrono::steady_clock().now();
		chrono::duration<double> frameDone = end - start;

		window.display();
		double delay = 1000 * ((1.0 / FPS) - frameDone.count());
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
				cout << "1x: " << pn.noise(x, y, z) << endl;
				// cout << "-1x: " << pn.noise(-x, -y, -z) << endl;
			}
		}		
	}
// */
	return 0;
}