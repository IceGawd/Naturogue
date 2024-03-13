#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <algorithm>
#include <vector>
#include <math.h>

using namespace std;

class RenderWindow;

template <class T>
struct Vector2 final {
	Vector2() : x(0), y(0) {}
	Vector2(T x, T y) :x(x), y(y) {}
	T x, y;
};

using Vector2f = Vector2<float>;

template <typename T>
Vector2<T> operator- (const Vector2<T>& l, const Vector2<T>& r);


void arrowChange(RenderWindow* window, vector<SDL_Scancode>& keys, bool* direction, void (*foo)(vector<void*>), vector<void*> passingArgument);
float angleBetween(int x1, int y1, int x2, int y2);

float dotProduct(Vector2f vec1, Vector2f vec2);
Vector2f vectorFromAngle(float angle);
Vector2f getMinMax(Vector2f vec, const vector<Vector2f>& points);
void addUnique(Vector2f vec, vector<float>& anglesToCheck);
bool collides(vector<Vector2f> polygon1, vector<Vector2f> polygon2);