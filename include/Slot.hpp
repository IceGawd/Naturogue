#pragma once

#include "Item.hpp"
#include "RenderWindow.hpp"

class Slot : public Entity {
public:
	Item* holding = nullptr;

	Slot(int x, int y, Item* h);
	void draw(RenderWindow* window);
};