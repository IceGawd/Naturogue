#include "Slot.hpp"

Slot::Slot(int x, int y, Item* h, SDL_Texture* texture) {
	if (texture != nullptr) {
		Slot::mainTexture = texture;
	}
	setTexture(Slot::mainTexture);
	this->x = x;
	this->y = y;
	holding = h;

	if (h != nullptr) {
		h->x = x;
		h->y = y;
	}
}

void Slot::draw(RenderWindow* window) {
	window->render(this, true);
	window->render(holding, true);
}