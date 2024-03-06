#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <vector>
#include <math.h>

using namespace std;

class RenderWindow;

void arrowChange(RenderWindow* window, vector<SDL_Scancode>& keys, bool* direction, void (*foo)(vector<void*>), vector<void*> passingArgument);
float angleBetween(int x1, int y1, int x2, int y2);