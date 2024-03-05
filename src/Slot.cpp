#include "Slot.hpp"

Slot::Slot(int x, int y, Item* h) {
	this->x = x;
	this->y = y;
	holding = h;

	/*
	if (h != nullptr) {
		h->x = x;
		h->y = y;
	}
	*/
}

void Slot::draw(RenderWindow* window) {
	window->render(this, true);

	if (holding != nullptr) {
		holding->slotDraw(window, this);
	}
}