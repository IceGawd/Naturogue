#include "Weapon.hpp"

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

	// /*
	rotationX = show_width * (xTemp / largestMod + 1) / 2;
	// rotationX = 0;
	rotationY = show_height * (yTemp / largestMod + 1) / 2;
	// */

	cout << "rx: " << rotationX << " ry: " << rotationY << endl;
	// */
}

bool Weapon::draw(RenderWindow* window, World* world, vector<GameObject*>& entities) {
	if (melee) {
		// GameObject::draw(window, world, entities);
		float angleBase = startAngle + item->itemData->swingAngle * (1.0 * framesAlive / item->itemData->swingTime - 0.5);
		angle = angleBase + item->itemData->angleMod - 3 * M_PI / 2;
		// angle = startAngle;
		// cout << startAngle << endl;
		framesAlive++;

		// TODO: MAYBE DECREASE DISTANCE FROM PLAYER (PLAYER DISTANCE IS PROPORTIONAL TO WEAPON SIZE)
		x = (RenderWindow::WIDTH) / 2 - rotationX;
		y = (RenderWindow::HEIGHT) / 2 - rotationY;

		float kbpower = item->itemData->swingAngle * (pow(show_width, 2) + pow(show_height, 2)) / (2 * item->itemData->swingTime);

		xvel = kbpower * cos(angleBase);
		yvel = kbpower * -sin(angleBase);

		points.clear();

		window->setColor(255, 0, 0, 255);

		for (Vector2f& v2f : item->itemData->points) {
			Vector2f toAdd = Vector2f(x, y);
			toAdd.x += rotationX + v2f.x * cos(angle);
			toAdd.y += rotationY + v2f.y * sin(angle);
			points.push_back(toAdd);

			window->drawLine(toAdd.x - 5, toAdd.y - 5, toAdd.x + 5, toAdd.y + 5);
			window->drawLine(toAdd.x - 5, toAdd.y + 5, toAdd.x + 5, toAdd.y - 5);
		}

		// x = RenderWindow::WIDTH / 2;
		// y = RenderWindow::HEIGHT / 2;

		// cout << "meleeeeeee\n";
		window->render(this, true);

		return framesAlive <= item->itemData->swingTime;
	}
	else {
		return false;
	}
}