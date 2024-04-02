#pragma once

#include "Item.hpp"
#include "GameObject.hpp"
#include "World.hpp"
#include "Player.hpp"

struct ItemDrop : public GameObject {
	Item* item;

	~ItemDrop();
	ItemDrop(int x, int y, Item* i, float a);
	virtual bool draw(RenderWindow* window, World* world, vector<GameObject*>& entities);
};
