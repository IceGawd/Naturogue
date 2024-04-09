#include "Difficulty.hpp"

#include <iostream>

using namespace std;

Difficulty::Difficulty() {

}

Difficulty::Difficulty(int x) {
	difficulty = x;
	iFrameBonus = pow((1 + 1.0 * (Difficulty::MAX_DIFFICULTY - difficulty) / Difficulty::MAX_DIFFICULTY), 2.5);

	for (int y = 0; y < OPTIONSLENGTH; y++) {
		options[y] = x % 2;
		cout << options[y] << endl;
		x /= 2;
	}
}

bool Difficulty::getOption(Options o) {
	// cout << (int) o << " " << (int) OPTIONSLENGTH << endl;
	return options[(int) o];
}