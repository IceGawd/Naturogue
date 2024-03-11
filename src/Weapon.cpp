#include "Weapon.hpp"

Weapon::Weapon(Item* i, bool m, float a) {
	item = i;
	melee = m;
	startAngle = a;

	xvel = 0;
	yvel = 0;

	texture = i->texture;
	fullPicSize();
	setRect();

	// /*
	rotationX = width / 2;
	rotationY = height;
	// */
}

bool Weapon::draw(RenderWindow* window, World* world, vector<GameObject*>& entities) {
	if (melee) {
		GameObject::draw(window, world, entities);
		angle = item->itemData->angleMod + startAngle + item->itemData->swingAngle * (1.0 * framesAlive / item->itemData->swingTime - 0.5);
		// angle = startAngle;
		// cout << startAngle << endl;
		framesAlive++;

		x = (RenderWindow::WIDTH - show_width) / 2;
		y = (RenderWindow::HEIGHT) / 2 - show_height;		

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