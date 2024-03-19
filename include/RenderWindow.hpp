#pragma once

#include <string>
#include <math.h>
#include <iostream>

#include "Entity.hpp"
#include "Button.hpp"
#include "ControlsConfig.hpp"

using namespace std;

struct World;

class RenderWindow {
public:
	const static int WIDTH = 1280;
	const static int HEIGHT = 720;
	const Uint8* keyboard;
	ControlsConfig cc;
	World* world;

	int actualWidth = WIDTH;
	int actualHeight = HEIGHT;
	double scaleMultiplier = 1;
	int xOrigin = 0;
	int yOrigin = 0;

	SDL_Window* window;
	SDL_Renderer* renderer;
	TTF_Font* sans;
	int x = 0;
	int y = 0;
	float zoom = 1.0;

	RenderWindow(const char* title); 
	void resizeWindow();
	SDL_Texture* loadTexture(const char* filePath);
	void cleanUp();
	void clear();
	SDL_Rect getDestRect(Entity* entity, bool stationary = false);
	void render(Entity* entity, bool stationary = false);
	void actualRender(Entity* entity, bool stationary = false);
	void render(Button* button);
	void display();
	void setColor(int r, int g, int b, int a);
	void cross(int x, int y, bool stationary = false);
	void drawLine(int x1, int y1, int x2, int y2);
	void drawCircle(int x, int y, int radius);
	void drawText(string text, unsigned char r, unsigned char g, unsigned char b, unsigned char a, int x, int y, int w, int h);
};