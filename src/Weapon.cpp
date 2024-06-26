#include "Weapon.hpp"

#include "World.hpp"
#include "Player.hpp"
#include "utils.hpp"
#include "ItemDrop.hpp"

Weapon::~Weapon() {
	if (!melee) {
		delete item;
	}
}

Weapon::Weapon(Item* i, bool m, float a, float c) {
	item = i;
	melee = m;
	startAngle = a;
	charge = c;

	xvel = 0;
	yvel = 0;

	damage = m ? i->itemData->meleeDamage : i->itemData->rangedDamage;
	damage *= c / 20;

	copyTexture(item->itemData);

	double xTemp = cos(item->itemData->angleMod);
	double yTemp = -sin(item->itemData->angleMod);
	double largestMod = max(abs(xTemp), abs(yTemp));


	rotationX = round(show_width * (xTemp / largestMod + 1) / 2);
	rotationY = round(show_height * (yTemp / largestMod + 1) / 2);

	if (!m) {
		xvel = item->itemData->throwVelocity * sin(a);
		yvel = item->itemData->throwVelocity * -cos(a);
		angle = a + item->itemData->angleMod - 3 * M_PI / 2;

		if (item->itemData->throwSpin) {
			rotationX = show_width / 2;
			rotationY = show_height / 2;
		}
	}
}

void Weapon::calculatePoints() {
	points.clear();

	for (Vector2f& v2f : item->itemData->points) {
		float radius = sqrt(pow(v2f.x - rotationX, 2) + pow(v2f.y - rotationY, 2));
		float pointAngle = pointAngleBetween(v2f.x, v2f.y, rotationX, rotationY) - angle; // TODO: why subtract angle?

		Vector2f toAdd = Vector2f(x + radius * cos(pointAngle) + rotationX, y - radius * sin(pointAngle) + rotationY);

		points.push_back(toAdd);

		// window->cross(toAdd.x, toAdd.y);
	}
}

bool Weapon::draw(RenderWindow* window, World* world, vector<GameObject*>& entities) {
	if (melee) {
		// GameObject::draw(window, world, entities);
		float percent = 1.0 * framesAlive / item->itemData->swingTime;

		if (item->itemData->hasProperty(SPEAR)) {
			angle = startAngle + item->itemData->angleMod - 3 * M_PI / 2;

			x = world->player->x - rotationX + world->player->show_width / 2 + sin(startAngle) * percent * show_width;
			y = world->player->y - rotationY + world->player->show_height / 2 - cos(startAngle) * percent * show_height;

			float kbpower = item->itemData->swingAngle * sqrt(pow(show_width, 2) + pow(show_height, 2)) / (2 * item->itemData->swingTime);

			// /*
			xvel = kbpower * sin(startAngle);
			yvel = -kbpower * cos(startAngle);
		}
		else {
			float angleBase = startAngle + item->itemData->swingAngle * (percent - 0.5);
			angle = angleBase + item->itemData->angleMod - 3 * M_PI / 2; // TODO: 3 * M_PI / 2 is up but thats not our 0 angle unless we do x y swap? 

			// angle = 0;
			// angle = startAngle;
			// cout << startAngle << endl;

			x = world->player->x - rotationX + world->player->show_width / 2;
			y = world->player->y - rotationY + world->player->show_height / 2;
			// cout << "rx: " << rotationX << " ry: " << rotationY << endl;
			// cout << x << " " << y << endl;

			/*
			window->setColor(0, 0, 255, 255);

			window->cross(x, y);
			window->cross(x + rotationX, y + rotationY);
			*/
			// */

			// cout << xvel << " "  << yvel << endl;
			float kbpower = item->itemData->swingAngle * sqrt(pow(show_width, 2) + pow(show_height, 2)) / (2 * item->itemData->swingTime);

			// /*
			xvel = kbpower * cos(angleBase);
			yvel = kbpower * sin(angleBase);
		}

		framesAlive++;

		calculatePoints();
		/*
		window->setColor(255, 0, 0, 255);
		for (Vector2f& v2f : points) {
			window->cross(v2f.x, v2f.y);
		}
		// */

		// x = RenderWindow::WIDTH / 2;
		// y = RenderWindow::HEIGHT / 2;

		// cout << "meleeeeeee\n";
		window->render(this);

		return framesAlive <= item->itemData->swingTime;
	}
	else {
		bool extendXP;
		bool extendXN;
		bool extendYP;
		bool extendYN;

		fixPreLoop(window, extendXP, extendXN, extendYP, extendYN);
		loopPreFix(this, extendXP, extendXN, extendYP, extendYN);

		if (framesAlive == 0) {
			x = world->player->x + world->player->show_width / 2 - rotationX;
			y = world->player->y + world->player->show_height / 2 - rotationY;
			xvel *= charge;
			yvel *= charge;
		}

		float velocity = sqrt(pow(xvel, 2) + pow(yvel, 2));

		if (item->itemData->throwSpin) {
			angle += velocity / 20 + 0.05;
		}

		framesAlive++;

		xvel *= item->itemData->friction;
		yvel *= item->itemData->friction;

		GameObject::draw(window, world, entities);

		calculatePoints();
		window->render(this);

		if (item->itemData->hasProperty(BOOMERANG)) {
			float velChange = (22 * atan((framesAlive - 71) / 75.6) + 17) * (1 - item->itemData->friction);
			// cout << "velChange: " << velChange << endl;

			float a = angleBetween(world->player);
			xvel -= velChange * cos(a);
			yvel += velChange * sin(a);

			if (comeBack) {
				if (distance(world->player) < 50) {
					velocity = 0;
					comeBack = false;
				}
			}
			else if (velocity <= 10) {
				comeBack = true;
			}
		}

		loopPostFix(this);
		if (velocity > 1 || comeBack) {
			return true;
		}
		else {
			if (item->itemData->hasProperty(DANGER)) {
				world->player->HP -= 50 / pow(2, numberOfDudesHit);
			}

			ItemDrop* id = new ItemDrop(x, y, item, angle);
			
			item = nullptr;
			id->rotationX = rotationX;
			id->rotationY = rotationY;

			entities.push_back(id);
			return false;
		}
	}
}