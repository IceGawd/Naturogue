#pragma once

#include "Item.hpp"
#include "RenderWindow.hpp"
#include "GameObject.hpp"

class Slot : public GameObject {
public:
	Item* holding = nullptr;
	static const int SIZE = 100;

	Slot(int x, int y, Item* h, SDL_Texture* slotTexture, SDL_Texture* selectedSlotTexture);
	virtual void changeSpriteSheet(string newSheet);
	void draw(RenderWindow* window);
};