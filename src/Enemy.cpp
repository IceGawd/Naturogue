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

	if (e->ai == SHRUB) {
		healthBar = new Bar(window, RenderWindow::WIDTH * 0.8, 70, 10, health, true, M_PI / 108.0);
	}
	else {
		healthBar = new Bar(window, show_width * 0.8, show_height * 0.3, 5, health);
	}

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
	Player* p = world->player;
	// cout << "start\n";

	// cout << "a2\n";
	// cout << "b2\n";

	float distanceFromPlayer = distance(p);

	if (random() < 0.01) {
		randoSampledX = x;
		randoSampledY = y;
	}

	// TODO: FOR AI PROGRAMMING KEEP IN MIND LOOPING WORLD (patched with utils.cpp's 3 loopFix functions)
	// /*

	if (distanceFromPlayer < world->renderSize) {
		if (ed->ai != SHRUB && health != ed->maxHP) {
			healthBar->x = x + (show_width - healthBar->show_width) / 2;
			healthBar->y = y - 2 * healthBar->show_height;
			healthBar->value = health;
			healthBar->draw(window, world, entities);
		}

		if (ed->ai == BOUNCING) {
			active = true;
			recoveryFrames--;
			animationType = 0;
			if (recoveryFrames < 0) {
				if (current == "Idle") {
					float angle;
					if (ed->aggroRange > distanceFromPlayer) {
						angle = angleBetween(p);
						for (Enemy* dude : group) {
							float angleChange = M_PI / (abs(dude->x - x) + abs(dude->y - y) + 1);
							angle += (random() + random() + random() - 1.5) * angleChange / 3;
						}
					}
					else {
						angle = 2 * M_PI * random();
					}
					
					recoveryFrames = ed->attackFrames;
					xvel -= cos(angle) * ed->movementspeed;
					yvel += sin(angle) * ed->movementspeed;
					// changeSpriteSheet("Bounce");
				}
				else {
					xvel = 0;
					yvel = 0;
					recoveryFrames = ed->attackdelay;
					// changeSpriteSheet("Idle");
				}
			}

			if (current == "Idle" && next->xFrames == 2) {
				animationFrame = 0;
				if (x + show_width / 2 < p->x + p->show_width / 2) {
					animationFrame = 1;
				}
				if (recoveryFrames < 10) {
					animationType = 1;
				}
			}
		}
		else if (ed->ai == SHRUB) {
			// cout << "shrubbin: " << x << " " << y << " " << this << endl;

			float speedMod = 1.5 - health / 2000;

			string facingString = (facing == LEFT) ? "L" : "R";

			if (p->x + show_width / 2 < x + show_width / 2) {
				facing = LEFT;
			}
			else {
				facing = RIGHT;
			}

			if (health <= 0) {
				if (current != "ShrubDeath") {
					changeSpriteSheet("ShrubDeath");
					rageMeter = 0;
				}
			}
			else if (current == "ShrubBattleStart" || active) {
				active = true;
				// changeSpriteSheet("ShrubBattleStart");
				if (rageMeter < 50) {
					y += 8;
					changeSpriteSheet("ShrubIdle" + facingString);
				}
				else {
					changeSpriteSheet("ShrubBattleStart");		
				}
				if (rageMeter == 100) {
					changeSpriteSheet("ShrubIdle" + facingString);
					active = false;
					rageMeter = 0;
				}
			}
			else {
				if (rageMeter == 0) {
					if (1 / (abs(randoSampledX - x) + abs(randoSampledY - y) + 1) > random()) {
						changeSpriteSheet("ShrubRun" + facingString);
						rage = true;						
					}
					else if (distanceFromPlayer < 200 && random() < 0.9) {
						changeSpriteSheet("ShrubAttack" + facingString);
					}
					else if (distanceFromPlayer < 400 && random() < 0.8) {
						changeSpriteSheet("ShrubRun" + facingString);
					}
					else if (distanceFromPlayer < 600 && random() < 0.7) {
						changeSpriteSheet("ShrubIdle" + facingString);
						rage = random() > 0.8 * health / ed->maxHP;
					}
					else if (distanceFromPlayer < 800 && random() < 0.6) {
						changeSpriteSheet("ShrubIdle" + facingString);
					}
					else {
						changeSpriteSheet("ShrubRun" + facingString);
						rage = random() < 0.6;
					}
				}
				else {
					float angle = angleBetween(p);
					if (current.substr(0, current.size() - 1) == "ShrubAttack") {
						changeSpriteSheet("ShrubAttack" + facingString);

						xvel -= cos(angle) * ed->movementspeed / 2 * speedMod;
						yvel += sin(angle) * ed->movementspeed / 2 * speedMod;
					}
					else if (current.substr(0, current.size() - 1) == "ShrubRun" && !rage) {
						changeSpriteSheet("ShrubRun" + facingString);

						xvel -= cos(angle) * ed->movementspeed * speedMod;
						yvel += sin(angle) * ed->movementspeed * speedMod;
					}
					else if (current.substr(0, current.size() - 1) == "ShrubIdle") {
						changeSpriteSheet("ShrubIdle" + facingString);
						xvel = 0;
						yvel = 0;
					}
					else if (current.substr(0, current.size() - 1) == "ShrubRun" && rage) {
						if ((int) (rageMeter) % 20 == 0) {
							// angle += (random() + random() + random() - 1.5) * M_PI / 6;
							angle = random() * M_PI * 2;
							xvel -= cos(angle) * ed->movementspeed * 30 * speedMod;
							yvel += sin(angle) * ed->movementspeed * 30 * speedMod;
						}
					}
					/*
					else if (current.substr(0, current.size() - 1) == "ShrubIdle" && rage) {
						changeSpriteSheet("ShrubIdle" + facingString);
						xvel = 0;
						yvel = 0;
						x = (p->x + world->structures[1].x) / 2;
						y = (p->y + world->structures[1].y - 200) / 2;
						rageMeter -= int(random() * 2);
					}
					*/

					if (next->xFrames * next->frames == rageMeter) {
						rageMeter = -1;
					}
				}
			}
			animationType = 0;
			animationFrame = (int) (rageMeter / next->frames) % next->xFrames;
			rageMeter++;
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

				if (animationType == 0) {
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

					if (1 / (abs(randoSampledX - x) + abs(randoSampledY - y) + 1) > random()) {
						DIRECTION next = DOWN;
						while (next == facing) {
							next = (DIRECTION) (4 * random());
						}
					}
					else if (!collided && (abs(flatX - x) / Block::BLOCKSIZE < 0.1 || abs(flatY - y) / Block::BLOCKSIZE < 0.1)) {
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
		else if (ed->ai == SPIN) {
			// cout << distanceFromPlayer << endl;
			if (ed->aggroRange > distanceFromPlayer) {
				bool curActive = ed->aggroRange > distanceFromPlayer;
				if (curActive && !active) {
					rageMeter = ed->attackFrames * distanceFromPlayer;
				}

				// cout << "Spin GameObject shit\n";
				active = curActive;
				x += xvel;
				y += yvel;
				bool collided = world->collides(this);

				// cout << "!collision check\n";
				if (!collided) {
					SDL_Rect me = getRect();
					for (Enemy* dude : group) {
						SDL_Rect you = dude->getRect();
						if (this != dude && SDL_HasIntersection(&me, &you) == SDL_TRUE) {
							// cout << x << " " << y << " " << xvel << " " << yvel << endl;

							x += (dude->x > x) ? -1 : 1;
							y += (dude->y > y) ? -1 : 1;
							dude->x += (dude->x > x) ? 1 : -1;
							dude->y += (dude->y > y) ? 1 : -1;

							xvel += (dude->x > x) ? -1 : 1;
							yvel += (dude->y > y) ? -1 : 1;
							dude->xvel += (dude->x > x) ? 1 : -1;
							dude->yvel += (dude->y > y) ? 1 : -1;
							collided = true;

							// cout << x << " " << y << " " << xvel << " " << yvel << endl;
							break;
						}
					}
				}

				// cout << "collision check\n";
				if (collided) {
					xvel = -xvel;
					yvel = -yvel;

					rage = !rage;
				}

				// cout << "angleStuff\n";
				float angle = (M_PI * rageMeter) / (2 * ed->attackFrames * distanceFromPlayer);
				if (rage) {
					angle *= -1;
				}
				angle += angleBetween(p);

				xvel -= ed->movementspeed * cos(angle);
				yvel += ed->movementspeed * sin(angle);

				// cout << "rageMeter\n";
				rageMeter -= distanceFromPlayer;
				if (rageMeter < 0) {
					rageMeter = ed->attackFrames * distanceFromPlayer;
				}

				// cout << "frame check\n";
				frames += sqrt(xvel * xvel + yvel * yvel) / (5 * (1 / (1 - ed->traction)) * ed->movementspeed);
				// cout << "frames: " << frames << endl;
				// cout << "ed->movementspeed: " << ed->movementspeed << endl;
				// cout << "30 * (1 / (1 - ed->traction)) * ed->movementspeed: " << 30 * (1 / (1 - ed->traction)) * ed->movementspeed << endl;
				while (frames > 4) {
					frames -= 4;
				}
				int actualFrame = int(frames);
				if (rage) {
					actualFrame = 3 - actualFrame;
				}

				// cout << "animation\n";
				animationFrame = actualFrame / 2;
				animationType = actualFrame % 2;
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
			// cout << "v2f.x: " << v2f.x << " v2f.y: " << v2f.y << endl;
			window->cross(v2f.x, v2f.y);
		}
		*/

		// cout << "you hit me check\n";
		if (invincibilityFrames == 0 && world->player->beingUsed.size() != 0) {
			for (Weapon* bu : world->player->beingUsed) {
				if (collides(window, hitbox, bu->points)) {
					bu->numberOfDudesHit++;

					invincibilityFrames = 10;
					float mod = 1;
					if (bu->item->itemData->hasProperty(TIPPER)) {
						mod *= 1.1 * atan((distance(world->player) - 156) / 60.3) + 1.3; 
						// cout << "mod: " << mod << endl;
					}
					if (bu->item->itemData->hasProperty(HAMMER)) {
						mod *= bu->charge / 10.0;						
					}

					float pab = pointAngleBetween(bu->xvel, bu->yvel, 0, 0);
					float kbx = bu->item->itemData->knockback * cos(pab) + bu->xvel;
					float kby = bu->item->itemData->knockback * -sin(pab) + bu->yvel;

					/*
					if (bu->melee) {
						kbx *= 2;
						kby *= 2;
					}
					*/

					kbx *= 1 - ed->knockbackResistance;
					kby *= 1 - ed->knockbackResistance;

					xvel += kbx * mod;
					yvel += kby * mod;

					int damageTaken = (bu->damage * mod) - ed->defence;
					if (damageTaken > 0) {
						health -= damageTaken;
					}

					if (!bu->melee && !bu->item->itemData->hasProperty(SPEAR) && world->d.getOption(WEAPONPIERCE)) {
						bu->xvel = 0;
						bu->yvel = 0;
					}
					// cout << "ouchie\n";
				}
				else {
					// cout << "x: " << x << " y: " << y << " wpbux: " << world->player->beingUsed->x << " wpbuy: " << world->player->beingUsed->y << endl;
				}
			}
		}

		// cout << "me hit you check\n";
		SDL_Rect me = getRect();
		SDL_Rect you = p->getRect();
		if (p->invincibilityFrames == 0 && SDL_HasIntersection(&me, &you) == SDL_TRUE) {
			// cout << "ed->damage: " << ed->damage << endl;
			// cout << "p->HP: " << player->HP << endl;
			p->invincibilityFrames = ed->damage;
			p->HP -= ed->damage;

			float angle = angleBetween(p);

			p->xvel -= cos(angle) * ed->damage;
			p->yvel += sin(angle) * ed->damage;
		}

		// cout << "invincibilitySHine\n";
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
			// cout << "renderin: " << this << endl;
			window->render(this);
		}
		if ((!active || ed->ai != SHRUB) && ed->ai != SPIN) {
			GameObject::draw(window, world, entities);
		}

		if (rageMeter == 30 && current == "ShrubDeath") {
			world->shrub = nullptr;
		}
	}

	return ((health <= 0 && ed->ai != SHRUB) || (rageMeter == 30 && current == "ShrubDeath"));
}