#include <iostream>

#include "GameObject.hpp"
#include "World.hpp"

using namespace std;

GameObject::GameObject() {
	
}

GameObject::GameObject(float px, float py, SDL_Texture* ptexture, int pw, int ph) : Entity(px, py, ptexture, pw, ph) {

}

bool GameObject::draw(RenderWindow* window, World* world, vector<GameObject*>& entities) {
	x += xvel;
	y += yvel;
	
	bool collided = world->collides(this);

	x -= int(x / World::WORLDLENGTH) * World::WORLDLENGTH;
	y -= int(y / World::WORLDLENGTH) * World::WORLDLENGTH;

	if (x < 0) {
		x += World::WORLDLENGTH;
	}
	if (y < 0) {
		y += World::WORLDLENGTH;
	}

	return false;
}

void GameObject::changeSpriteSheet(string newSheet) {
	current = newSheet;
	next = &(sheets[current]);

	animationType = 0;
	animationFrame = 0;
	angle = 0;

	texture = next->texture;
	SDL_Point p = getsize();

	width = p.x / next->xFrames;
	height = p.y / next->yFrames;
}

GameObject* GameObject::createObject() {
	return new GameObject(*this);
}