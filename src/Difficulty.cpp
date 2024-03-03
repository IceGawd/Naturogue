#include "Difficulty.hpp"

Difficulty::Difficulty(int x) {
	x -= 1;
	for (int y = 0; y < OPTIONSLENGTH; y++) {
		options[y] = x % 2;
		x /= 2;
	}
}