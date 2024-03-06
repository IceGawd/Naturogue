#include "utils.hpp"
#include "RenderWindow.hpp"

void arrowChange(RenderWindow* window, vector<SDL_Scancode>& keys, bool* direction, void (*foo)(vector<void*>), vector<void*> passingArgument) {
	bool unpressed = true;
	for (SDL_Scancode key : keys) {
		if (window->keyboard[key]) {
			if (!(*direction)) {
				if (foo != nullptr) {
					foo(passingArgument);
				}
				*direction = true;
			}
			unpressed = false;
		}
	}
	if (unpressed) {
		*direction = false;
	}
}

float angleBetween(int x1, int y1, int x2, int y2) {
	float angle;
	float xDiff = x1 - x2;
	float yDiff = y1 - y2;
	if (xDiff == 0) {
		if (yDiff >= 0) {
			angle = M_PI / -2.0;
		}
		else {
			angle = M_PI / 2.0;			
		}
	}
	else if (xDiff > 0) {
		angle = -atan(yDiff / xDiff);
	}
	else {
		angle = M_PI - atan(yDiff / xDiff);
	}
	return angle;
}
