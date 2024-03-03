#pragma once

enum Options {
	increasedStatus, 
	movableHome, 
	defensiveOptions, 
	projectileSpeed, 
	lessSlots, 
	increasedStats, 
	newMoves, 
	enemiesRespawn, 
	OPTIONSLENGTH
};

struct Difficulty {
	bool options[OPTIONSLENGTH];
	Difficulty(int x);
};