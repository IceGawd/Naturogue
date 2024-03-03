#pragma once

struct Input {
	bool right = false;
	bool left = false;
	bool up = false;
	bool down = false;

	inline bool diagonal() { return (up || down) && (right || left); }
};