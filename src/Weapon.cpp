#include "Weapon.hpp"

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
	hitboxRotateX = round(width * (xTemp / largestMod + 1) / 2);
	hitboxRotateY = round(height * (yTemp / largestMod + 1) / 2);

	// cout << "rx: " << rotationX << " ry: " << rotationY << endl;
	// */
}

bool Weapon::draw(RenderWindow* window, World* world, vector<GameObject*>& entities) {
	if (melee) {
		// GameObject::draw(window, world, entities);
		float angleBase = startAngle + item->itemData->swingAngle * (1.0 * framesAlive / item->itemData->swingTime - 0.5);
		angle = angleBase + item->itemData->angleMod - 3 * M_PI / 2;

		// angle = 0;
		// angle = startAngle;
		// cout << startAngle << endl;
		framesAlive++;

		x = (RenderWindow::WIDTH) / 2 - rotationX;
		y = (RenderWindow::HEIGHT) / 2 - rotationY;

		window->setColor(0, 0, 255, 255);

		window->cross(x, y);
		window->cross(x + rotationX, y + rotationY);

		float kbpower = item->itemData->swingAngle * (pow(show_width, 2) + pow(show_height, 2)) / (2 * item->itemData->swingTime);

		xvel = kbpower * cos(angleBase);
		yvel = kbpower * -sin(angleBase);

		points.clear();

		window->setColor(255, 0, 0, 255);

		for (Vector2f& v2f : item->itemData->points) {
			float radius = sqrt(pow(v2f.x - rotationX, 2) + pow(v2f.y - rotationY, 2));
			float pointAngle = pointAngleBetween(v2f.x, v2f.y, rotationX, rotationY) - angle;

			Vector2f toAdd = Vector2f(x + radius * cos(pointAngle) + rotationX, y - radius * sin(pointAngle) + rotationY);

			points.push_back(toAdd);

			window->cross(toAdd.x, toAdd.y);
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