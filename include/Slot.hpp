#pragma once

#include "Item.hpp"
#include "RenderWindow.hpp"

class Slot : public Entity {
public:
	static SDL_Texture* mainTexture;

	Item* holding = nullptr;

	Slot(int x, int y, Item* h, SDL_Texture* texture = nullptr);
	void draw(RenderWindow* window);
};