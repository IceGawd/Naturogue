#include "Button.hpp"

#include <string>

using namespace std;

Button::~Button() {
	SDL_DestroyTexture(message);
}

Button::Button(RenderWindow* window, string n, int px, int py, int pw, int ph, unsigned char r1, unsigned char g1, unsigned char b1, unsigned char r2, unsigned char g2, unsigned char b2, bool s, void (*f)(vector<void*>)) : name(n), show(s) {
	x = px;
	y = py;
	width = pw;
	height = ph;
	show_width = pw;
	show_height = ph;

	foo = f;

	area = {px, py, pw, ph};

	SDL_Surface* surf = SDL_CreateRGBSurfaceWithFormat(0, pw, ph, 32, SDL_PIXELFORMAT_ARGB8888);
	setTexture(SDL_CreateTextureFromSurface(window->renderer, surf));
	setRect();

	Uint32* pixels = (Uint32*) surf->pixels;

	Uint8 r;
	Uint8 g;
	Uint8 b;
	Uint8 a;

	for (int y = 0; y < surf->h; y++) {
		for (int x = 0; x < surf->w; x++) {
			int index = y * width + x;
			if ((x > width - THICKNESS) || (x < THICKNESS) || (y > height - THICKNESS) || (y < THICKNESS)) {
				r = 0;
				g = 0;
				b = 0;
				a = 255;
			}
			else {
				r = r1;
				g = g1;
				b = b1;
				a = 255;
			}
			pixels[index] = SDL_MapRGBA(surf->format, r, g, b, a);
		}
	}

	SDL_UpdateTexture(texture.get(), NULL, pixels, surf->pitch);

	SDL_Surface* surfaceMessage = TTF_RenderText_Solid(window->sans, n.c_str(), {r2, g2, b2, 255});
	message = SDL_CreateTextureFromSurface(window->renderer, surfaceMessage);

	// SDL_FreeSurface(surfaceMessage);

	float wScale = 1.0 * (pw - 2 * THICKNESS) / surfaceMessage->w;
	float hScale = 1.0 * (ph - 2 * THICKNESS) / surfaceMessage->h;

	if (wScale < hScale) {
		textArea.w = surfaceMessage->w * wScale;
		textArea.h = surfaceMessage->h * wScale;
		textArea.x = px + THICKNESS;
		textArea.y = py + THICKNESS + (ph - textArea.h) / 2;
	}
	else {
		textArea.w = surfaceMessage->w * hScale;
		textArea.h = surfaceMessage->h * hScale;
		textArea.x = px + THICKNESS + (pw - textArea.w) / 2;
		textArea.y = py + THICKNESS;
	}
}

void Button::draw(RenderWindow* window, vector<void*> passingArgument) {
	if (show) {
		// cout << x << " " << y << endl;
		window->render(this, true);
		SDL_RenderCopy(window->renderer, message, NULL, &textArea);

		if (clicked) {
			foo(passingArgument);
		}
	}
	clicked = false;
}