#pragma once

enum Options {
	ENEMYKNOCKBACK, //	1
	WEAPONPIERCE, //	2
	BONUSITEMSLOTS, //	4
	ENEMYSPEED, //		8
	ENEMYDAMAGE, //		16
	DONTDOUBLEMAXHP, //	32
	ENEMYHP, //			64
	NOREGEN, //			128
	OPTIONSLENGTH
};

struct Difficulty {
	constexpr static int MAX_DIFFICULTY = (1 << OPTIONSLENGTH) - 1;
	bool options[OPTIONSLENGTH];
	Difficulty();
	Difficulty(int x);

	bool getOption(Options o);
};