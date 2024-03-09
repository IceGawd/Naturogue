#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <map>
#include <string>

#include "Entity.hpp"
#include "RenderWindow.hpp"
#include "SpriteSheet.hpp"

class World;

using namespace std;

class GameObject : public Entity {
public:
	GameObject();
	GameObject(float px, float py, SDL_Texture* ptexture, int pw, int ph);
	
	float xvel = 0;
	float yvel = 0;

	float frames = 0;
	string current;
	SpriteSheet* next;
	map<string, SpriteSheet> sheets;

	virtual bool draw(RenderWindow* window, World* world, vector<GameObject*>& entities);
	virtual void changeSpriteSheet(string newSheet);
	virtual GameObject* createObject();
};