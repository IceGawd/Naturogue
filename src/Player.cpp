#include "Player.hpp"

Player::Player(RenderWindow* window, shared_ptr<SDL_Texture>& slotTexture, shared_ptr<SDL_Texture>& selectedSlotTexture) {
	for (int x = 0; x < slots; x++) {
		// cout << "w: " << w << " h: " << h << endl;
		Slot s(x * Slot::SIZE + (RenderWindow::WIDTH - slots * Slot::SIZE) / 2, RenderWindow::HEIGHT - Slot::SIZE, nullptr, slotTexture, selectedSlotTexture);

		items.push_back(s);
	}

	select(0);

	sheets["down"] = SpriteSheet(window->loadTexture("res/gfx/James_Downward.png"), 2, 2, 20);
	sheets["up"] = SpriteSheet(window->loadTexture("res/gfx/James_Upward.png"), 2, 2, 20);

	changeSpriteSheet("down");

	// /*
	show_width = width * 5 / 32;
	show_height = height * 5 / 32;
	// */
	/*
	show_width = width / 7;
	show_height = height / 7;
	*/
}

void Player::select(int num) {
	items[selectedSlot].changeSpriteSheet("Slot");
	items[num].changeSpriteSheet("Selected");

	selectedSlot = num;
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

	if (mousedown || (animation && swing)) {
		xvel *= chargeTraction;
		yvel *= chargeTraction;
	}
	if (mousedown) {
		charge++;
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

	if (animation) {
		// cout << "drawign\n";
		bool alive = beingUsed->draw(window, world, entities);
		if (!alive) {
			delete beingUsed;
			beingUsed = nullptr;
			animation = false;
			swing = false;
			yeet = false;
		}
	}
	else {
		if (swing) {
			// do attack lol
			beingUsed = new Weapon(items[selectedSlot].holding, true, attackAngle);
			animation = true;
		}
	}

	for (Slot& s : items) {
		s.draw(window);
	}

	return false;
}