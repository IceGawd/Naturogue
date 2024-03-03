#pragma once

#include <string>
#include <map>

#include "SpriteSheet.hpp"
#include "Difficulty.hpp"

using namespace std;

enum AI {
	NONE, 
	BOUNCING
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
	AI ai;
	map<string, SpriteSheet> sheets;
	string startSheetName;

	EnemyData(string n, int da, int mo, int af, int ad, int ma, int de, float t, float k, AI ai, map<string, SpriteSheet> sh, string st) {
		damage = da;
		movementspeed = mo;
		attackFrames = af;
		attackdelay = ad;
		maxHP = ma;
		defence = de;
		traction = t;
		knockbackResistance = k;
		this->ai = ai;
		sheets = sh;
		startSheetName = st;
	}
};