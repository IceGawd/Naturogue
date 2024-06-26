#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <algorithm>
#include <vector>
#include <math.h>

using namespace std;

class RenderWindow;
class Entity;

template <class T>
struct Vector2 final {
	Vector2() : x(0), y(0) {}
	Vector2(T x, T y) :x(x), y(y) {}
	T x, y;

	Vector2<T> operator+ (const Vector2<T>& r) {
		return { x + r.x, y + r.y };
	}

	Vector2<T> operator- (const Vector2<T>& r) {
		return { x - r.x, y - r.y };
	}

	Vector2<T> operator* (const T& r) {
		return {T(round(x * r)), T(round(y * r))};
	}
};


using Vector2f = Vector2<float>;

pair<int, int> operator- (const pair<int, int>& l, const pair<int, int>& r);
pair<int, int> operator+ (const pair<int, int>& l, const pair<int, int>& r);

vector<Vector2f> square(float x, float y, float w, float h);
void arrowChange(RenderWindow* window, vector<SDL_Scancode>& keys, bool* direction, void (*foo)(vector<void*>), vector<void*> passingArgument);
float pointAngleBetween(int x1, int y1, int x2, int y2);

float dotProduct(Vector2f vec1, Vector2f vec2);
Vector2f vectorFromAngle(float angle);
Vector2f getMinMax(Vector2f vec, const vector<Vector2f>& points);
void addUnique(Vector2f vec, vector<float>& anglesToCheck);
bool collides(RenderWindow* window, vector<Vector2f> polygon1, vector<Vector2f> polygon2);

void fixPreLoop(RenderWindow* window, bool& extendXP, bool& extendXN, bool& extendYP, bool& extendYN);
void loopPreFix(Entity* go, bool extendXP, bool extendXN, bool extendYP, bool extendYN);
void loopPostFix(Entity* go);

enum WEAPON_PROPERTIES {
	BOOMERANG, 
	TIPPER, 
	HAMMER, 
	SPEAR, 
	DANGER
};

enum TRIGGER {
	NO_TRIGGER, 
	BOSS
};

enum SCREEN {
	MAIN, 
	DIFFICULTYSELECT, 
	GAME, 
	GAMEOVER, 
	WINCREDITS
};