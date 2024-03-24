#include "Enemy.hpp"
#include "World.hpp"

Enemy::Enemy(int x, int y, EnemyData* e) {
	this->x = x;
	this->y = y;
	this->ed = e;
	this->health = e->maxHP;
	this->sheets = e->sheets;

	changeSpriteSheet(e->startSheetName);

	show_width = width / 7;
	show_height = height / 7;

	cout << "width: " << width << endl;
	cout << "height: " << height << endl;
}

bool Enemy::draw(RenderWindow* window, World* world, vector<GameObject*>& entities) {
	// cout << "start\n";
	GameObject::draw(window, world, entities);

	// cout << "a2\n";
	Player* p = static_cast<Player*>(entities[0]);
	// cout << "b2\n";

	// TODO: FOR AI PROGRAMMING KEEP IN MIND LOOPING WORLD
	if (ed->ai == BOUNCING) {
		recoveryFrames--;
		if (recoveryFrames < 0) {
			if (current == "Idle") {
				float angle = angleBetween(p);
				
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

		xvel *= ed->traction;
		yvel *= ed->traction;
	}
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

				float pab = pointAngleBetween(bu->xvel, bu->yvel, 0, 0);
				float kbx = bu->item->itemData->knockback * cos(pab) + bu->xvel;
				float kby = bu->item->itemData->knockback * -sin(pab) + bu->yvel;

				xvel += kbx;
				yvel += kby;
				// cout << "ouchie\n";
			}
			else {
				// cout << "x: " << x << " y: " << y << " wpbux: " << world->player->beingUsed->x << " wpbuy: " << world->player->beingUsed->y << endl;
			}
		}
	}

	if (invincibilityFrames > 0) {
		// cout << "invincibilityFrames: " << invincibilityFrames << endl;
		invincibilityFrames--;
	}

	setRect();
	
	window->render(this);

	return false;
}