#include "Slot.hpp"

Slot::Slot(int x, int y, Item* h, SDL_Texture* slotTexture, SDL_Texture* selectedSlotTexture) {
	this->x = x;
	this->y = y;
	holding = h;

	// cout << slotTexture.get() << endl;
	// cout << selectedSlotTexture.get() << endl;

	sheets = {
		{"Slot", SpriteSheet(slotTexture, 1, 1, 1)}, 
		{"Selected", SpriteSheet(selectedSlotTexture, 1, 1, 1)}, 
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