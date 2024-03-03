#pragma once

#include "Entity.hpp"
#include "ItemData.hpp"

struct Item : public Entity {
	ItemData* itemData;
	Item(ItemData* id);
};