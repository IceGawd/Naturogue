#include "ItemDrop.hpp"

ItemDrop::ItemDrop(int x, int y, Item* i, float a) {
	this->x = x;
	this->y = y;

	item = i;
	angle = a;

	copyTexture(item->itemData);
}

bool ItemDrop::draw(RenderWindow* window, World* world, vector<GameObject*>& entities) {
	float dist = distance((Entity*) world->player);

	if (dist < 300) {
		float ab = angleBetween((Entity*) world->player);

		xvel -= dist * cos(ab) / 30;
		yvel += dist * sin(ab) / 30;
	}

	xvel *= 0.8;
	yvel *= 0.8;

	GameObject::draw(window, world, entities);

	window->render(this);

	SDL_Rect me = getRect();
	SDL_Rect you = world->player->getRect();	

	SDL_bool intersected = SDL_HasIntersection(&me, &you);
	bool die = (intersected == SDL_TRUE);
	if (intersected == SDL_TRUE) {
		die = world->player->giveItem(item);
	}

	return die;
}