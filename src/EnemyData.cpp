#include "EnemyData.hpp"
#include "World.hpp"

EnemyData::EnemyData(string n, int da, int mo, int af, int ad, int ma, int de, float t, float k, AI ai, map<string, SpriteSheet> sh, string st, int ar, int w, int h, World* world) {
	name = n;
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
	aggroRange = ar;
	show_width = w;
	show_height = h;

	if (!world->d.getOption(ENEMYDAMAGE)) {
		damage /= 1.5;
		damage++;
	}
	if (!world->d.getOption(ENEMYSPEED)) {
		movementspeed /= 1.5;
		movementspeed++;
	}
	else {
		attackFrames *= 0.5;
		attackdelay *= 0.5;
	}
	if (world->d.getOption(ENEMYKNOCKBACK)) {
		knockbackResistance = 1.5 * knockbackResistance + 0.1;
	}
	if (world->d.getOption(ENEMYHP)) {
		maxHP *= 1.5;
		defence *= 1.5;
		defence += 1;
	}
}