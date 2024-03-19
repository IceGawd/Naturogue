#include "Weapon.hpp"

#include "World.hpp"
#include "Player.hpp"
#include "utils.hpp"

Weapon::Weapon(Item* i, bool m, float a) {
	item = i;
	melee = m;
	startAngle = a;

	xvel = 0;
	yvel = 0;

	copyTexture(item->itemData);

	// /*
	double xTemp = cos(item->itemData->angleMod);
	double yTemp = -sin(item->itemData->angleMod);

	double largestMod = max(abs(xTemp), abs(yTemp));

	/*
	rotationX = show_width / 2;
	rotationY = show_height;
	*/

	rotationX = round(show_width * (xTemp / largestMod + 1) / 2);
	rotationY = round(show_height * (yTemp / largestMod + 1) / 2);

	// */
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
		float angleBase = startAngle + item->itemData->swingAngle * (1.0 * framesAlive / item->itemData->swingTime - 0.5);
		angle = angleBase + item->itemData->angleMod - 3 * M_PI / 2; // TODO: 3 * M_PI / 2 is up but thats not our 0 angle unless we do x y swap? 

		// angle = 0;
		// angle = startAngle;
		// cout << startAngle << endl;
		framesAlive++;

		x = world->player->x - rotationX + world->player->show_width / 2;
		y = world->player->y - rotationY + world->player->show_height / 2;
		// cout << "rx: " << rotationX << " ry: " << rotationY << endl;
		// cout << x << " " << y << endl;

		/*
		window->setColor(0, 0, 255, 255);

		window->cross(x, y);
		window->cross(x + rotationX, y + rotationY);
		*/

		float kbpower = item->itemData->swingAngle * sqrt(pow(show_width, 2) + pow(show_height, 2)) / (2 * item->itemData->swingTime);

		// /*
		xvel = kbpower * cos(angleBase);
		yvel = kbpower * sin(angleBase);
		// */

		// cout << xvel << " "  << yvel << endl;

		window->setColor(255, 0, 0, 255);

		calculatePoints();
		/*
		for (Vector2f& v2f : points) {
			window->cross(v2f.x, v2f.y);
		}
		*/

		// x = RenderWindow::WIDTH / 2;
		// y = RenderWindow::HEIGHT / 2;

		// cout << "meleeeeeee\n";
		window->render(this);

		return framesAlive <= item->itemData->swingTime;
	}
	else {
		// GameObject::draw(window, world, entities);		
		if (item->itemData->throwSpin) {

		}
		else {

		}

		calculatePoints();
		return false;
	}
}