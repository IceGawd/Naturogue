#include "Weapon.hpp"

Weapon::Weapon(Item* i, bool m, float a) {
	item = i;
	melee = m;
	startAngle = a;

	xvel = 0;
	yvel = 0;

	copyTexture(item->itemData);

	// /*
	double xTemp = cos(item->itemData->angleMod) / 2.0;
	double yTemp = -sin(item->itemData->angleMod) / 2.0;

	double largestMod = max(abs(xTemp), abs(yTemp));

	/*
	rotationX = show_width / 2;
	rotationY = show_height;
	*/

	// /*
	rotationX = show_width * (xTemp / largestMod + 0.5);
	// rotationX = 0;
	rotationY = show_height * (yTemp / largestMod + 0.5);
	// */

	cout << "rx: " << rotationX << " ry: " << rotationY << endl;
	// */
}

bool Weapon::draw(RenderWindow* window, World* world, vector<GameObject*>& entities) {
	if (melee) {
		GameObject::draw(window, world, entities);
		angle = startAngle + item->itemData->swingAngle * (1.0 * framesAlive / item->itemData->swingTime - 0.5);
		// angle = startAngle;
		// cout << startAngle << endl;
		framesAlive++;

		x = (RenderWindow::WIDTH) / 2 - rotationX;
		y = (RenderWindow::HEIGHT) / 2 - rotationY;

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