#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <string>
#include <vector>

#include "RenderWindow.hpp"
#include "Entity.hpp"

using namespace std;

class Button : public Entity {
public:
	const int THICKNESS = 10;

	SDL_Rect area;
	SDL_Rect textArea;

	bool clicked = false;
	bool show;
	string name;

	SDL_Texture* message;
	void (*foo)(vector<void*>);

	~Button();
	Button(RenderWindow* window, string n, int px, int py, int pw, int ph, unsigned char r1, unsigned char g1, unsigned char b1, unsigned char r2, unsigned char g2, unsigned char b2, bool s, void (*f)(vector<void*>));
	void draw(RenderWindow* window, vector<void*> passingArgument);
};