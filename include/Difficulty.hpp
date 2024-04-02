#pragma once

enum Options {
	ENEMYDAMAGE, 
	ENEMYKNOCKBACK,
	ENEMYSPEED, 
	WEAPONPIERCE,
	BONUSITEMSLOTS,
	DONTDOUBLEMAXHP,
	ENEMYHP,
	NOREGEN,
	OPTIONSLENGTH
};

struct Difficulty {
	constexpr static int MAX_DIFFICULTY = (1 << OPTIONSLENGTH) - 1;
	bool options[OPTIONSLENGTH];
	Difficulty();
	Difficulty(int x);

	bool getOption(Options o);
};