#include <SDL2/SDL_mixer.h>

#include <chrono>

#include "PerlinNoise.hpp"
#include "Button.hpp"
#include "Player.hpp"
#include "World.hpp"
#include "Enemy.hpp"
#include "EnemyData.hpp"

using namespace std;

/*
REMOVED
Decreased Player movement acceleration (- speed + traction) [max speed = speed * traction / (1 - traction)], attack speed, and smaller pickup range

1 - Enemies inflict more statuses and for longer
2 - Enemies can change their home location
4 - Some enemies can Block, Spotdodge, Roll and/or Reflect
8 - Increased projectile speed
16 - You gain 1 extra slot for items instead of 2 per level up
32 - Increased enemy hp, defence, movement speed, attack speed, damage, knockback resistance
64 - Enemies and Bosses get new moves
128 - Enemies respawn
*/

SDL_Texture* Slot::mainTexture;

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
	};

	// Mix_Music* backgroundMusic = OST["Void"];

	bool gameRunning = true;
	bool playingMusic = true;
	SDL_Event event;

	RenderWindow window("Naturogue");
	World* world = new World(&window);
	Player* player = new Player(&window);

	vector<Button*> buttons;
	vector<GameObject*> entities;

	vector<EnemyData*> enemyDatas = {
		new EnemyData("Flowy", 5, 40, 500, 60, 30, 0, 0.9, 0, BOUNCING, {
			{"Bounce", SpriteSheet(window.loadTexture("res/gfx/Flowy.png"), 1, 1, 1)}, 
			{"Idle", SpriteSheet(window.loadTexture("res/gfx/Flowy.png"), 1, 1, 1)}
		}, "Idle")
	};

	vector<ItemData*> itemDatas = {
		new ItemData(window.loadTexture("knife.png"), )
	}

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
			}
			SDL_Keycode kc = event.key.keysym.sym;
			if (event.type == SDL_KEYDOWN) {
				if (kc == SDLK_w) {
					player->input.up = true;
				}
				if (kc == SDLK_s) {
					player->input.down = true;
				}
				if (kc == SDLK_a) {
					player->input.left = true;
				}
				if (kc == SDLK_d) {
					player->input.right = true;
				}
			}
			if (event.type == SDL_KEYUP) {
				if (kc == SDLK_w) {
					player->input.up = false;
				}
				if (kc == SDLK_s) {
					player->input.down = false;
				}
				if (kc == SDLK_a) {
					player->input.left = false;
				}
				if (kc == SDLK_d) {
					player->input.right = false;
				}
			}
		}

		world->draw(&window);
		for (GameObject* go : entities) {
			go->draw(&window, world, entities);
		}

		for (Button* b : buttons) {
			if (b->show) {
				window.render(b);
			}
		}

		// CAMERA
		window.x = (window.x + player->x - RenderWindow::WIDTH / 2) / 2;
		window.y = (window.y + player->y - RenderWindow::HEIGHT / 2) / 2;

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
				cout << "-1x: " << pn.noise(-x, -y, -z) << endl;
			}
		}		
	}
*/
	return 0;
}