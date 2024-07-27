#pragma once

#include <math.h>

enum Options {
	ENEMYKNOCKBACK, //	1
	WEAPONPIERCE, //	2
	ENEMYDAMAGE, //		4
	DONTDOUBLEMAXHP, //	8
	BONUSITEMSLOTS, //	16
	ENEMYSPEED, //		32
	ENEMYHP, //			64
	MOREENEMIES, //		128
	NOREGEN, //			256
	OPTIONSLENGTH //    TOTAL: 511
};

struct Difficulty {
	constexpr static int MAX_DIFFICULTY = (1 << OPTIONSLENGTH) - 1;
	bool options[OPTIONSLENGTH];
	int difficulty;
	float iFrameBonus;
	Difficulty();
	Difficulty(int x);

	bool getOption(Options o);
};