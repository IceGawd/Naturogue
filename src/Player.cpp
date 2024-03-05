#include "Player.hpp"

Player::Player(RenderWindow* window, shared_ptr<SDL_Texture>& slotTexture) {
	int w;
	int h;
	SDL_QueryTexture(slotTexture.get(), NULL, NULL, &w, &h);

	w /= 4;
	h /= 4;

	for (int x = 0; x < slots; x++) {
		// cout << "w: " << w << " h: " << h << endl;
		Slot s(x * w + (RenderWindow::WIDTH - slots * w) / 2, RenderWindow::HEIGHT - h, nullptr);
		s.texture = slotTexture;

		s.fullPicSize();
		s.show_width = w;
		s.show_height = h;
		s.setRect();
		items.push_back(s);
	}

	sheets["down"] = SpriteSheet(window->loadTexture("res/gfx/James_Downward.png"), 2, 2, 20);
	sheets["up"] = SpriteSheet(window->loadTexture("res/gfx/James_Upward.png"), 2, 2, 20);

	changeSpriteSheet("down");

	speed = 6;
	traction = 0.7;

	// /*
	show_width = width * 5 / 32;
	show_height = height * 5 / 32;
	// */
	/*
	show_width = width / 7;
	show_height = height / 7;
	*/
}

bool Player::draw(RenderWindow* window, World* world, vector<GameObject*>& entities) {
	GameObject::draw(window, world, entities);

	double diagDirect = (input.diagonal()) ? 1 / sqrt(2) : 1;

	if (input.up) {
		if (current != "up") {
			changeSpriteSheet("up");
		}
		yvel -= speed * diagDirect;
	}
	if (input.down) {
		if (current != "down") {
			changeSpriteSheet("down");
		}
		yvel += speed * diagDirect;		
	}
	if (input.left) {
		xvel -= speed * diagDirect;		
	}
	if (input.right) {
		xvel += speed * diagDirect;
	}

	xvel *= traction;
	yvel *= traction;

	float currSpeed = sqrt(pow(xvel, 2) + pow(yvel, 2));
	float ch = currSpeed / (speed * traction / (1 - traction));
	frames += ch;
	while (frames >= next->frames) {
		frames -= next->frames;
	}
	if (currSpeed < speed * traction) {
		frames = 0;
	}

	animationType = next->yFrames * frames / next->frames;
	animationFrame = int(next->xFrames * next->yFrames * frames / next->frames) % next->xFrames;

	setRect();
	window->render(this);

	for (Slot& s : items) {
		s.draw(window);
	}

	return false;
}