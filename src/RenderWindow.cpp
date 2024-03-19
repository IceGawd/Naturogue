#include "RenderWindow.hpp"
#include "World.hpp"

using namespace std;

RenderWindow::RenderWindow(const char* title) : window(NULL), renderer(NULL) {
	window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);

	if (window == NULL) {
		cout << "Window failed to Init: " << SDL_GetError() << "\n"; 
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	keyboard = SDL_GetKeyboardState(NULL);
	resizeWindow();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	sans = TTF_OpenFont("res/gfx/zephyrea.ttf", 24);
};

void RenderWindow::resizeWindow() {
	SDL_GetWindowSize(window, &actualWidth, &actualHeight);
	double wScale = 1.0 * actualWidth / WIDTH;
	double hScale = 1.0 * actualHeight / HEIGHT;

	cout << "actualWidth: " << actualWidth << " actualHeight: " << actualHeight << endl;
	scaleMultiplier = min(wScale, hScale);
	xOrigin = (actualWidth - (WIDTH * scaleMultiplier)) / 2;
	yOrigin = (actualHeight - (HEIGHT * scaleMultiplier)) / 2;
}

void RenderWindow::cleanUp() {
	SDL_DestroyWindow(window);
}

SDL_Texture* RenderWindow::loadTexture(const char* filePath) {
	SDL_Texture* texture = IMG_LoadTexture(renderer, filePath);

	if (texture == NULL) {
		cout << "Failed to load a texture " << SDL_GetError() << "\n";
	}

	return texture;
}

void RenderWindow::clear() {
	SDL_RenderClear(renderer);
}

void RenderWindow::setColor(int r, int g, int b, int a) {
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

void RenderWindow::cross(int x, int y, bool stationary) {
	if (!stationary) {
		x = (x - this->x) * zoom;
		y = (y - this->y) * zoom;
	}
	drawLine(x - 5, y - 5, x + 5, y + 5);
	drawLine(x - 5, y + 5, x + 5, y - 5);
}

void RenderWindow::drawLine(int x1, int y1, int x2, int y2) {
	SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

SDL_Rect RenderWindow::getDestRect(Entity* entity, bool stationary) {
	SDL_Rect dest;
	if (stationary) {
		dest = entity->getRect();
	}
	else {
		dest.x = (int) ((entity->x - x) * zoom);
		dest.y = (int) ((entity->y - y) * zoom);
		dest.w = (int) (entity->show_width * zoom);
		dest.h = (int) (entity->show_height * zoom);
	}
	return dest;
}

void RenderWindow::render(Entity* entity, bool stationary) {
	if (stationary) {
		actualRender(entity, true);
	}
	else {
		SDL_Rect windowRect = {x, y, int(RenderWindow::WIDTH / zoom), int(RenderWindow::HEIGHT / zoom)};
		SDL_Rect temp;

		for (int z = 0; z < 9; z++) {
			int xChange = (z % 3 - 1) * World::WORLDLENGTH;
			int yChange = (z / 3 - 1) * World::WORLDLENGTH;

			SDL_Rect modLoc = entity->getRect();
			modLoc.x += xChange;
			modLoc.y += yChange;

			if (SDL_IntersectRect(&windowRect, &modLoc, &temp) == SDL_TRUE) {
				entity->x += xChange;
				entity->y += yChange;

				actualRender(entity, false);
				
				entity->x -= xChange;
				entity->y -= yChange;
			}
		}
	}
}

void RenderWindow::actualRender(Entity* entity, bool stationary) {
	if (entity->textureDraw) {
		SDL_Rect dest = getDestRect(entity, stationary);
		SDL_Rect src = entity->currentFrame;
		SDL_Point center = SDL_Point();

		if (entity->rotationX == -1) {
			center.x = src.w / 2;
		}
		else {
			center.x = entity->rotationX * zoom;
		}
		if (entity->rotationY == -1) {
			center.y = src.h / 2;
		}
		else {
			center.y = entity->rotationY * zoom;
		}

		SDL_RendererFlip flip = SDL_FLIP_NONE;
		if (entity->flip) {
			flip = SDL_FLIP_HORIZONTAL;
		}

		SDL_RenderCopyEx(renderer, entity->texture.get(), &src, &dest, 180 * entity->angle / M_PI, &center, flip);
	}
	entity->customDraw(this);
}

void RenderWindow::render(Button* button) {
	SDL_Rect r = button->area;
	SDL_Color tc = button->textcolor;
	SDL_Color bc = button->buttoncolor;

	setColor(bc.r, bc.g, bc.b, bc.a);
	SDL_RenderFillRect(renderer, &r);
	drawText(button->name, tc.r, tc.g, tc.b, tc.a, r.x, r.y, r.w, r.h);
}

void RenderWindow::display() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	int w;
	int h;
	int r1x;
	int r1y;
	if (xOrigin == 0) { // TOP AND BOTTOM
		w = actualWidth;
		h = yOrigin;
		r1x = 0;
		r1y = actualHeight - h;
	}
	else { // LEFT AND RIGHT
		w = xOrigin;
		h = actualHeight;
		r1x = actualWidth - w;
		r1y = 0;
	}
	SDL_Rect r1 = {0, 0, w, h};
	SDL_RenderFillRect(renderer, &r1);
	r1 = {r1x, r1y, w + 1, h};
	SDL_RenderFillRect(renderer, &r1);
	SDL_RenderPresent(renderer);
}

void RenderWindow::drawCircle(int x, int y, int radius) {
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	for (int w = 0; w < radius * 2; w++) {
		for (int h = 0; h < radius * 2; h++) {
			int dx = radius - w; // horizontal offset
			int dy = radius - h; // vertical offset
			if ((dx * dx + dy * dy) <= (radius * radius)) {
				SDL_RenderDrawPoint(renderer, x + dx, y + dy);
			}
		}
	}
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}

void RenderWindow::drawText(string text, unsigned char r, unsigned char g, unsigned char b, unsigned char a, int x, int y, int w, int h) {

	// as TTF_RenderText_Solid could only be used on
	// SDL_Surface then you have to create the surface first
//	cout << 1 << endl;
	SDL_Color color = {r, g, b, a};
//	cout << 1.5 << endl;
//	cout << SDL_GetError() << endl;
//	cout << TTF_GetError() << endl;
	SDL_Surface* surfaceMessage = TTF_RenderText_Solid(sans, text.c_str(), color); 
//	cout << 2 << endl;

	// now you can convert it into a texture
	SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
//	cout << 3 << endl;

	SDL_Rect Message_rect; //create a rect
	Message_rect.x = x;  //controls the rect's x coordinate 
	Message_rect.y = y; // controls the rect's y coordinte
	Message_rect.w = w; // controls the width of the rect
	Message_rect.h = h; // controls the height of the rect

	// (0,0) is on the top left of the window/screen,
	// think a rect as the text's box,
	// that way it would be very simple to understand

	// Now since it's a texture, you have to put RenderCopy
	// in your game loop area, the area where the whole code executes

	// you put the renderer's name first, the Message,
	// the crop size (you can ignore this if you don't want
	// to dabble with cropping), and the rect which is the size
	// and coordinate of your texture
	SDL_RenderCopy(renderer, Message, NULL, &Message_rect);

	// Don't forget to free your surface and texture
	SDL_FreeSurface(surfaceMessage);
	SDL_DestroyTexture(Message);
}