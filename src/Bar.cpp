#include "Bar.hpp"
#include "RenderWindow.hpp"

Bar::Bar(RenderWindow* window, int w, int h, int t, float mv) {
	width = w;
	height = h;
	show_width = w;
	show_height = h;
	thickness = t;
	maxValue = mv;

	// /*
	// */
	/*
	x = 0;
	y = 0;
	*/

	// SDL_SetSurfaceBlendMode(surf, SDL_BLENDMODE_BLEND);
	surf = SDL_CreateRGBSurfaceWithFormat(0, w, h, 32, SDL_PIXELFORMAT_ARGB8888);

	/*
	cout << "surf->format->format: " << surf->format->format << endl;
	cout << "surf->format->BitsPerPixel: " << int(surf->format->BitsPerPixel) << endl;
	cout << "surf->format->BytesPerPixel: " << int(surf->format->BytesPerPixel) << endl;
	cout << "surf->format->Rmask: " << surf->format->Rmask << endl;
	cout << "surf->format->Gmask: " << surf->format->Gmask << endl;
	cout << "surf->format->Bmask: " << surf->format->Bmask << endl;
	cout << "surf->format->Amask: " << surf->format->Amask << endl;
	*/

	// surf = TTF_RenderText_Blended(window->sans, "1", {255, 255, 255, 255});

	/*
	cout << "surf->format->format: " << surf->format->format << endl;
	cout << "surf->format->BitsPerPixel: " << int(surf->format->BitsPerPixel) << endl;
	cout << "surf->format->BytesPerPixel: " << int(surf->format->BytesPerPixel) << endl;
	cout << "surf->format->Rmask: " << surf->format->Rmask << endl;
	cout << "surf->format->Gmask: " << surf->format->Gmask << endl;
	cout << "surf->format->Bmask: " << surf->format->Bmask << endl;
	cout << "surf->format->Amask: " << surf->format->Amask << endl;
	*/
	
	setTexture(SDL_CreateTextureFromSurface(window->renderer, surf));

	setRect();
}

bool Bar::draw(RenderWindow* window, World* world, vector<GameObject*>& entities) {
	// cout << "waveMod: " << waveMod << endl;
	// cout << "surf->format->format: " << surf->format->format << endl;

	waveMod += WAVEDELTA;
	Uint32* pixels = (Uint32*) surf->pixels;

	Uint8 r;
	Uint8 g;
	Uint8 b;
	Uint8 a;

	for (int y = 0; y < surf->h; y++) {
		for (int x = 0; x < surf->w; x++) {
			// /*
			int index = y * width + x;
			float mod = waveMod + (y + x) * M_PI / 180;
			if ((x > width - thickness) || (x < thickness) || (y > height - thickness) || (y < thickness)) {
				r = 0;
				g = 0;
				b = 0;
				a = 255;
			}
			else if ((x - thickness) / (width - 2.0 * thickness) < (value / maxValue)) {
				r = (Uint8) (127.5 * sin(mod) + 127.5);
				g = (Uint8) (127.5 * sin(mod + 2 * M_PI / 3) + 127.5);
				b = (Uint8) (127.5 * sin(mod + 4 * M_PI / 3) + 127.5);
				a = 255;
			}
			else {
				a = 0;
			}
			pixels[index] = SDL_MapRGBA(surf->format, r, g, b, a);
			// */

			/*
			int index = y * surf->w + x;
			float mod = waveMod + (entDist + y + x) * M_PI / 180;
			SDL_GetRGBA(pixels[index], surf->format, &r, &g, &b, &a);
			r = (Uint8) (127.5 * sin(mod) + 127.5);
			g = (Uint8) (127.5 * sin(mod + 2 * M_PI / 3) + 127.5);
			b = (Uint8) (127.5 * sin(mod + 4 * M_PI / 3) + 127.5);
			a = 255;
			pixels[index] = SDL_MapRGBA(surf->format, r, g, b, a);
			*/
		}
	}

	SDL_UpdateTexture(texture.get(), NULL, pixels, surf->pitch);

	window->render(this);

	return false;
}