#pragma once

#include "Entity.hpp"
#include "ItemData.hpp"

struct Slot;

struct Item : public Entity {
	const double SLOTSCALER = 0.8;
	ItemData* itemData;

	Item(ItemData* id);
	void slotDraw(RenderWindow* window, Slot* s);
};