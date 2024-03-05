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