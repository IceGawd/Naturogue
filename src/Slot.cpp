#include "Slot.hpp"

Slot::Slot(int x, int y, Item* h, shared_ptr<SDL_Texture>& slotTexture, shared_ptr<SDL_Texture>& selectedSlotTexture) {
	this->x = x;
	this->y = y;
	holding = h;

	sheets = {
		{"Slot", SpriteSheet(slotTexture.get(), 1, 1, 1)}, 
		{"Selected", SpriteSheet(selectedSlotTexture.get(), 1, 1, 1)}, 
	};

	changeSpriteSheet("Slot");

	/*
	if (h != nullptr) {
		h->x = x;
		h->y = y;
	}
	*/
}

void Slot::changeSpriteSheet(string newSheet) {
	GameObject::changeSpriteSheet(newSheet);

	fullPicSize();
	show_width = Slot::SIZE;
	show_height = Slot::SIZE;
	setRect();
}

void Slot::draw(RenderWindow* window) {
	window->render(this, true);

	if (holding != nullptr) {
		holding->slotDraw(window, this);
	}
}