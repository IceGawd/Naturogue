#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <memory>

#include <math.h>

class RenderWindow;

using namespace std;

struct sdl_deleter
{
	void operator()(SDL_Texture *p) const {
		SDL_DestroyTexture(p);
	}
};

class Entity {
public:
	Entity();
	// ~Entity();
	Entity(float px, float py, SDL_Texture* ptexture);
	Entity(float px, float py, SDL_Texture* ptexture, int pw, int ph);
	// Entity(const Entity& e);
	void setRect();
	void fullPicSize();
	SDL_Point getsize();
	SDL_Rect getRect();
	void setTexture(SDL_Texture* ptexture);
	float distance(Entity* e);
	float angleBetween(Entity* e);
	virtual void customDraw(RenderWindow* window);

	float x;
	float y;
	int width;
	int height;
	int show_width;
	int show_height;
	bool textureDraw = true;
	float angle = 0;
	bool flip = false;
	
	int animationFrame = 0;
	int animationType = 0;
	int modifier = 0;
	SDL_Rect currentFrame;
	shared_ptr<SDL_Texture> texture;
};