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
	setRect();

	
	window->render(this);

	return false;
}