#include <iostream>

#include "GameObject.hpp"
#include "World.hpp"

using namespace std;

GameObject::~GameObject() {
	// cout << "GAMEOBJECT DESTROY: " << this << endl;
}

GameObject::GameObject() {
	
}

GameObject::GameObject(float px, float py, SDL_Texture* ptexture, int pw, int ph) : Entity(px, py, ptexture, pw, ph) {

}

bool GameObject::draw(RenderWindow* window, World* world, vector<GameObject*>& entities) {
	if (abs(xvel) >= Block::BLOCKSIZE / 2) {
		xvel = (Block::BLOCKSIZE * xvel) / (abs(xvel) * 2);
	}
	if (abs(yvel) >= Block::BLOCKSIZE / 2) {
		yvel = (Block::BLOCKSIZE * yvel) / (abs(yvel) * 2);
	}

	x += xvel;
	y += yvel;
	
	bool collided = world->collides(this);

	/*
	if (x < 0) {
		x += World::WORLDLENGTH;
	}
	if (y < 0) {
		y += World::WORLDLENGTH;
	}
	*/

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