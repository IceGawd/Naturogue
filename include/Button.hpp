#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <string>

using namespace std;

class Button {
public:
	SDL_Rect area;
	SDL_Color textcolor;
	SDL_Color buttoncolor;
	bool clicked = false;
	bool show;
	string name;

	Button(string n, int px, int py, int pw, int ph, unsigned char r1, unsigned char g1, unsigned char b1, unsigned char r2, unsigned char g2, unsigned char b2, bool s);
};