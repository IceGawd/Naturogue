#include "Enemy.hpp"
#include "World.hpp"

// TODO: MAKE VIRTUAL DESTRUCTOR
Enemy::~Enemy() {
	// /*
	// cout << "deleting Enemy\n";
	delete healthBar;
	for (Enemy* dude : group) {
		if (dude != this) {
			// cout << "prevDudeGroupSize: " << dude->group.size() << endl;
			dude->group.erase(remove(dude->group.begin(), dude->group.end(), this));
			// cout << "nowDudeGroupSize: " << dude->group.size() << endl;
		}
	}
	// */
}

Enemy::Enemy(int x, int y, EnemyData* e, RenderWindow* window) {
	this->x = x;
	this->y = y;
	this->ed = e;
	this->health = e->maxHP;
	this->sheets = e->sheets;

	changeSpriteSheet(e->startSheetName);

	show_width = e->show_width;
	show_height = e->show_height;

	healthBar = new Bar(window, show_width * 0.8, show_height * 0.3, 5, health);

	// cout << "width: " << width << endl;
	// cout << "height: " << height << endl;
}

void Enemy::pixelEdit(void (*func)(Uint8&, Uint8&, Uint8&, Uint8&)) {
	Uint32* pixels = nullptr;
	int pitch = 0;
	Uint32 format;
	int w, h;
	SDL_QueryTexture(texture.get(), &format, nullptr, &w, &h);

	if (SDL_LockTexture(texture.get(), nullptr, (void**)&pixels, &pitch)) {
		SDL_GetError();
		// cout << "TEXTURE LOCK!!!!!!!!!!\n";
	}

	SDL_PixelFormat pixelFormat;
	pixelFormat.format = format;

	Uint8 r;
	Uint8 g;
	Uint8 b;
	Uint8 a;

	for (int x = animationFrame * width; x < (animationFrame + 1) * width; x++) {
		for (int y = animationType * height; y < (animationType + 1) * height; y++) {
			Uint32 pixelPosition = y * (pitch / sizeof(unsigned int)) + x;

			SDL_GetRGBA(pixels[pixelPosition], &pixelFormat, &r, &g, &b, &a);

			func(r, g, b, a);

			Uint32 color = SDL_MapRGBA(&pixelFormat, r, g, b, a);
			pixels[pixelPosition] = color;
		}
	}

	SDL_UnlockTexture(texture.get());
}

void light(Uint8& r, Uint8& g, Uint8& b, Uint8& a) {
	r = 255 - ((255 - r) / 2);
	g = 255 - ((255 - g) / 2);
	b = 255 - ((255 - b) / 2);
}

void dark(Uint8& r, Uint8& g, Uint8& b, Uint8& a) {
	r = 255 - (2 * (255 - r));
	g = 255 - (2 * (255 - g));
	b = 255 - (2 * (255 - b));
}

void Enemy::lighten() {
	pixelEdit(light);
}

void Enemy::darken() {
	pixelEdit(dark);
}


bool Enemy::draw(RenderWindow* window, World* world, vector<GameObject*>& entities) {
	if (health != ed->maxHP) {
		healthBar->x = x + (show_width - healthBar->show_width) / 2;
		healthBar->y = y - 2 * healthBar->show_height;
		healthBar->value = health;
		healthBar->draw(window, world, entities);
	}

	// cout << "start\n";

	// cout << "a2\n";
	Player* p = world->player;
	// cout << "b2\n";

	float distanceFromPlayer = distance(p);

	// TODO: FOR AI PROGRAMMING KEEP IN MIND LOOPING WORLD (patched with utils.cpp's 3 loopFix functions)
	// /*

	if (distanceFromPlayer < 5000) {
		GameObject::draw(window, world, entities);
		if (ed->ai == BOUNCING) {
			active = true;
			recoveryFrames--;
			if (recoveryFrames < 0) {
				if (current == "Idle") {
					float angle;
					if (ed->aggroRange > distanceFromPlayer) {
						angle = angleBetween(p);
					}
					else {
						angle = 2 * M_PI * random();
					}
					
					recoveryFrames = ed->attackFrames;
					xvel -= cos(angle) * ed->movementspeed;
					yvel += sin(angle) * ed->movementspeed;
					changeSpriteSheet("Bounce");
				}
				else {
					xvel = 0;
					yvel = 0;
					recoveryFrames = ed->attackdelay;
					changeSpriteSheet("Idle");
				}
			}
		}
		else if (ed->ai == SNEAKING) {
			// active = true;
			int midX = (x + show_width / 2) / Block::BLOCKSIZE;
			int midY = (y + show_height / 2) / Block::BLOCKSIZE;
			onTopOf = world->blocks[index(midY, midX)];

			/*
			window->setColor(255, 0, 0, 255);
			window->cross(onTopOf->x, onTopOf->y);
			window->cross(onTopOf->x + onTopOf->show_width, onTopOf->y);
			window->cross(onTopOf->x + onTopOf->show_width, onTopOf->y + onTopOf->show_height);
			window->cross(onTopOf->x, onTopOf->y + onTopOf->show_height);
			*/

			int flatX = Block::BLOCKSIZE * round(x / Block::BLOCKSIZE);
			int flatY = Block::BLOCKSIZE * round(y / Block::BLOCKSIZE);

			// cout << "x: " << x << " y: " << y << endl;

			if (findHidingSpot && onTopOf->isSolidGrass()) {
				findHidingSpot = false;
			}

			bool prevActive = active;
			active = true;

			// cout << group.size() << endl;
			for (Enemy* dude : group) {
				float d = dude->distance(p);
				if ((rage && 3 * ed->aggroRange < d) || (!rage && ((prevActive && 2 * ed->aggroRange < d) || (!prevActive && ed->aggroRange < d)))) {
					active = false;
					rage = false;
					// cout << "I don't wanna go\n";
				}
				// cout << "D: " << d << endl;
				rageMeter += 1000000 / (d * d) / group.size();
			}
			if (rageMeter > 0) {
				rageMeter -= group.size() * random();
			}
			else {
				rageMeter = 0;
			}
			// cout << "RAGE: " << rageMeter << endl;


			if (rageMeter > 100 || invincibilityFrames > 0) {
				for (Enemy* dude : group) {
					dude->rage = true;
					dude->active = true;
					rageMeter -= group.size() * random();
				}
			}

			if (findHidingSpot || rage || active) {
				if (current != "Walking") {
					changeSpriteSheet("Walking");
				}

				bool collided = false;
				if (!findHidingSpot) {
					SDL_Rect me = getRect();
					for (Enemy* dude : group) {
						SDL_Rect you = dude->getRect();
						if (this != dude && SDL_HasIntersection(&me, &you) == SDL_TRUE && facing == dude->facing) {
							// cout << "hlelp\n";
							DIRECTION next = UP;
							while (next == facing) {
								next = (DIRECTION) (4 * random());
							}
							facing = next;
							collided = true;
						}
					}
				}

				if (!collided && (abs(flatX - x) / Block::BLOCKSIZE < 0.1 || abs(flatY - y) / Block::BLOCKSIZE < 0.1)) {
					float angle;
					if (findHidingSpot) {
						int r = 1;
						while (hidingSpot == nullptr) {
							// cout << "lag cause\n";
							for (int x = -r; x <= r; x++) {
								for (int y = -r; y <= r; y++) {
									if (abs(x) == r || abs(y) == r) {
										Block* b = world->blocks[index(midY + y, midX + x)];
										// cout << b->type->name.substr(0, 7) << " " << b->type->name.substr(0, 11) << endl;
										if (b->isSolidGrass()) {
											// cout << "FOUND<<<<<<<<<<\n";
											hidingSpot = b;
											break;
										}
									}
								}		
							}
							r++;
						}
						angle = angleBetween(hidingSpot);
					}
					else {
						angle = angleBetween(p);
					}
					if (abs(angle) < M_PI / 4) {
						facing = RIGHT;
					}
					else if (angle < 3 * M_PI / 4 && angle > M_PI / 4) {
						facing = DOWN;
					}
					else if (angle < 5 * M_PI / 4 && angle > 3 * M_PI / 4) {
						facing = LEFT;
					}
					else {
						facing = UP;
					}
				}
				if (facing == UP) {
					yvel -= ed->movementspeed;
					animationFrame = 3;
				}
				else if (facing == DOWN) {
					yvel += ed->movementspeed;
					animationFrame = 0;
				}
				else if (facing == LEFT) {
					xvel += ed->movementspeed;
					animationFrame = 1;
				}
				else if (facing == RIGHT) {
					xvel -= ed->movementspeed;
					animationFrame = 2;
				}
				animationType = (animationType + 1) % 7;
				// cout << animationType << endl;
			}
			else {
				x = flatX;
				y = flatY;
				xvel = 0;
				yvel = 0;
				if (current != "Hidden") {
					changeSpriteSheet("Hidden");
					animationFrame = 0;
					animationType = 4;
				}
				hidingSpot = nullptr;
				if (!onTopOf->isSolidGrass()) {
					findHidingSpot = true;
				}
			}
		}	
		// */			

		xvel *= ed->traction;
		yvel *= ed->traction;
		// cout << "end\n";

		vector<Vector2f> hitbox = square(x, y, show_width, show_height);

		/*
		window->setColor(0, 0, 255, 255);
		for (Vector2f& v2f : hitbox) {
			cout << "v2f.x: " << v2f.x << " v2f.y: " << v2f.y << endl;
			window->cross(v2f.x, v2f.y);
		}
		*/

		if (invincibilityFrames == 0 && world->player->beingUsed.size() != 0) {
			for (Weapon* bu : world->player->beingUsed) {
				if (collides(window, hitbox, bu->points)) {
					invincibilityFrames = 10;
					float mod = 1;
					if (bu->item->itemData->hasProperty(TIPPER)) {
						mod *= 1.1 * atan((distance(world->player) - 156) / 60.3) + 1.3; 
						// cout << "mod: " << mod << endl;
					}

					float pab = pointAngleBetween(bu->xvel, bu->yvel, 0, 0);
					float kbx = bu->item->itemData->knockback * cos(pab) + bu->xvel;
					float kby = bu->item->itemData->knockback * -sin(pab) + bu->yvel;

					kbx *= 1 - ed->knockbackResistance;
					kby *= 1 - ed->knockbackResistance;

					xvel += kbx * mod;
					yvel += kby * mod;

					int damageTaken = (bu->damage * mod) - ed->defence;
					if (damageTaken > 0) {
						health -= damageTaken;
					}
					// cout << "ouchie\n";
				}
				else {
					// cout << "x: " << x << " y: " << y << " wpbux: " << world->player->beingUsed->x << " wpbuy: " << world->player->beingUsed->y << endl;
				}
			}
		}

		if (invincibilityFrames > 0) {
			// cout << "invincibilityFrames: " << invincibilityFrames << endl;

			// lighter = (invincibilityFrames % 2 == 0) ? lighter : !lighter;
			lighter = !lighter;
			/*
			if (lighter) {
				lighten();
			}
			*/

			invincibilityFrames--;
		}
		else if (lighter) {
			// darken();
			lighter = false;
		}

		setRect();
		
		if (!lighter) {
			window->render(this);
		}
	}

	return health <= 0;
}