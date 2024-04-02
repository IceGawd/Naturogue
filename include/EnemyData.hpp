#pragma once

#include <string>
#include <map>

#include "SpriteSheet.hpp"
#include "Difficulty.hpp"

struct World;

using namespace std;

enum AI {
	NONE, 
	BOUNCING, 
	SNEAKING, 
	SHRUB, 
	SPIN
};

enum DIRECTION {
	UP, 
	DOWN, 
	RIGHT, 
	LEFT
};

struct EnemyData {
	string name;
	int damage;
	int movementspeed;
	int attackFrames;
	int attackdelay;
	int maxHP;
	int defence;
	float traction;
	float knockbackResistance;
	int aggroRange;
	int show_width;
	int show_height;
	AI ai;
	map<string, SpriteSheet> sheets;
	string startSheetName;

	EnemyData(string n, int da, int mo, int af, int ad, int ma, int de, float t, float k, AI ai, map<string, SpriteSheet> sh, string st, int ar, int w, int h, World* world);
};