#include <iostream>

#include "Entity.hpp"

using namespace std;

/*
Entity::~Entity() {
	// cout << "lmao gone" << endl;
	SDL_DestroyTexture(texture.get());
}
*/

Entity::Entity() : x(0), y(0) {

}

Entity::Entity(float px, float py, SDL_Texture* ptexture, int pw, int ph) : x(px), y(py), width(pw), height(ph), show_width(pw), show_height(ph) {
	setTexture(ptexture);
	setRect();
}

Entity::Entity(float px, float py, SDL_Texture* ptexture) : x(px), y(py) {
	setTexture(ptexture);
	fullPicSize();
	setRect();
}

void Entity::setTexture(SDL_Texture* ptexture) {
	texture = shared_ptr<SDL_Texture>(ptexture, sdl_deleter());
	SDL_SetTextureBlendMode(ptexture, SDL_BLENDMODE_BLEND);
}

// COPY CONSTRUCTOR
/*
Entity::Entity(const Entity& e) {
	cout << "COPY" << endl;

	texture = new SDL_Texture();
	*texture = *(e.texture);
}
*/

void Entity::fullPicSize() {
	SDL_Point p = getsize();
	width = p.x;
	show_width = p.x;
	height = p.y;
	show_height = p.y;
}

void Entity::setRect() {
	currentFrame.x = animationFrame * width;
	currentFrame.y = animationType * height;
	currentFrame.w = width;
	currentFrame.h = height;
}

SDL_Point Entity::getsize() {
    SDL_Point size;
    SDL_QueryTexture(texture.get(), NULL, NULL, &size.x, &size.y);
    return size;
}

SDL_Rect Entity::getRect() {
	return {(int) x, (int) y, show_width, show_height};
}

float Entity::distance(Entity* e) {
	double xDiff = pow(x - e->x, 2);
	double yDiff = pow(y - e->y, 2);
	// cout << "xDiff: " << xDiff << endl;
	// cout << "yDiff: " << yDiff << endl;
	return sqrt(xDiff + yDiff);
}

float Entity::angleBetween(Entity* e) {
	float angle;
	float xDiff = x - e->x;
	float yDiff = y - e->y;
	if (xDiff == 0) {
		if (yDiff >= 0) {
			angle = M_PI / -2.0;
		}
		else {
			angle = M_PI / 2.0;			
		}
	}
	else if (xDiff > 0) {
		angle = -atan(yDiff / xDiff);
	}
	else {
		angle = M_PI - atan(yDiff / xDiff);
	}
	return angle;
}

void Entity::customDraw(RenderWindow* window) {
	
}