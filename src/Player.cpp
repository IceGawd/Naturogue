#include "Player.hpp"
#include "ItemDrop.hpp"

Player::Player(RenderWindow* window, SDL_Texture* slotTexture, SDL_Texture* selectedSlotTexture) {
	this->slotTexture = slotTexture;
	this->selectedSlotTexture = selectedSlotTexture;

	// cout << "pst: " << slotTexture << endl;
	// cout << "psst: " << selectedSlotTexture << endl;

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

	chargeBar = new Bar(window, 160, 50, 5, 20);
	healthBar = new Bar(window, 160, 50, 5, MAXIMUM_HP, true);
	healthBar->x = 0;
	healthBar->y = RenderWindow::HEIGHT - healthBar->show_height;
	// chargeBar->x = (RenderWindow::WIDTH - chargeBar->width - show_width) / 2;
	// chargeBar->y = RenderWindow::HEIGHT / 2 - chargeBar->height - show_height;
	// chargeBar->x = (RenderWindow::WIDTH - chargeBar->width) / 2;
	// chargeBar->y = (RenderWindow::HEIGHT - chargeBar->height) / 2 - show_height;
}

void Player::readyToPlay(World* world) {
	// cout << "maxHP?\n";
	// cout << "world: " << world << endl;
	// cout << "prev: " << actualMaxHp << endl;
	if (world->d.getOption(DONTDOUBLEMAXHP)) {
		actualMaxHp = MAXIMUM_HP;
		// cout << "after1: " << actualMaxHp << endl;
	}
	else {
		actualMaxHp = 2 * MAXIMUM_HP;
		// cout << "after2: " << actualMaxHp << endl;
	}
	// cout << "setHPs???\n";
	HP = actualMaxHp;
	// cout << "is maxvaliue the issue???\n";
	// cout << actualMaxHp << " " << healthBar->maxValue << endl;
	healthBar->maxValue = (float) actualMaxHp;
	// cout << "healthBar->maxValue: " << healthBar->maxValue << endl;

	// cout << "itemClear\n";
	while (!items.empty()) {
		if (items.at(0).holding != nullptr) {
			delete items.at(0).holding;
		}

		items.erase(items.begin());
	}

	// cout << "slots\n";	
	slots = 3;
	if (!world->d.getOption(BONUSITEMSLOTS)) {
		slots = 5;
	}

	// cout << slotTexture.get() << endl;
	// cout << selectedSlotTexture.get() << endl;

	for (int x = 0; x < slots; x++) {
		Slot s(x * Slot::SIZE + (RenderWindow::WIDTH - slots * Slot::SIZE) / 2, RenderWindow::HEIGHT - Slot::SIZE, nullptr, slotTexture, selectedSlotTexture);

		items.push_back(s);
	}

	select(0);
}

void Player::select(int num) {
	items[selectedSlot].changeSpriteSheet("Slot");
	items[num].changeSpriteSheet("Selected");

	selectedSlot = num;
}

bool Player::giveItem(Item* item) {
	for (Slot& s : items) {
		if (s.holding == nullptr) {
			s.holding = item;
			return true;
		}
	}
	return false;
}

float Player::valueFromCharge() {
	return 20 + exp(-0.05 * charge - 0.15) * (-21 + charge * (-1 + charge * -0.02));
}

void interact(vector<void*> object) {
	World* world = (World*) (object[0]);
	Player* player = (Player*) (object[1]);
	vector<ItemData*>* itemDatas = (vector<ItemData*>*) (object[2]);
	vector<GameObject*>* entities = (vector<GameObject*>*) (object[3]);

	int midX = (player->x + player->show_width / 2) / Block::BLOCKSIZE;
	int midY = (player->y + player->show_height / 2) / Block::BLOCKSIZE;

	vector<pair<int, int>> areasToCheck;

	if (player->current == "up" || player->input.up) {
		areasToCheck.push_back({midX, midY - 1});
	}
	if (player->current == "down" || player->input.down) {
		areasToCheck.push_back({midX, midY + 1});
	}
	if (player->current == "left" || player->input.left) {
		areasToCheck.push_back({midX - 1, midY});
	}
	if (player->current == "right" || player->input.right) {
		areasToCheck.push_back({midX + 1, midY});
	}

	for (Block* block : world->blocks) {
		for (pair<int, int> pii : areasToCheck) {
			string name = block->type->name;
			if (name.substr(0, 5) == "Chest" && name.substr(name.size() - 6, 6) == "Closed" && block->x == pii.first * Block::BLOCKSIZE && block->y == pii.second * Block::BLOCKSIZE) {
				block->switchBlockType(world->getBlockData(name.substr(0, name.size() - 6) + "Open"));
				entities->push_back(new ItemDrop(block->x + Block::BLOCKSIZE / 2, block->y + Block::BLOCKSIZE / 2, new Item((*itemDatas)[(int) (random() * itemDatas->size())]), 0));
			}
		}
	}
}

bool Player::draw(RenderWindow* window, World* world, vector<GameObject*>& entities) {
	GameObject::draw(window, world, entities);

	double diagDirect = (input.diagonal()) ? 1 / sqrt(2) : 1;
	healthBar->value = HP;

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
		// chargeBar->value = 20 + exp(-0.025 * charge - 0.15) * (-21 + charge * (-0.5 + charge * -0.005));
		chargeBar->value = valueFromCharge();
		chargeBar->x = x + (show_width - chargeBar->show_width) / 2;
		chargeBar->y = y - 2 * chargeBar->show_height;
		chargeBar->draw(window, world, entities);

		// cout << "charge: " << charge << endl;

		charge++;
	}

	Block* block = world->blocks[index((y + 3 * show_height / 4) / Block::BLOCKSIZE, (x + show_width / 2) / Block::BLOCKSIZE)];
	float traction = block->type->traction;

	/*
	window->setColor(255, 0, 0, 255);
	window->cross(block->x, block->y);
	window->cross(block->x + block->show_width, block->y);
	window->cross(block->x + block->show_width, block->y + block->show_height);
	window->cross(block->x, block->y + block->show_height);
	*/

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

	timeWithoutDamage++;
	if (invincibilityFrames > 0) {
		if (timeWithoutDamage > 0) {
			timeWithoutDamage = 0;
		}
		timeWithoutDamage -= invincibilityFrames;
		invincibilityFrames--;
	}
	lighter = invincibilityFrames % 2 == 0;
	if (!world->d.getOption(NOREGEN)) {
		HP += random() * pow(timeWithoutDamage / 100, 0.2);
		if (HP > actualMaxHp) {
			HP = actualMaxHp;
		}
	}

	if (lighter) {
		window->render(this);
	}

	if (!animation) {
		if (swing || yeet) {
			// do attack lol
			if (items[selectedSlot].holding != nullptr) {
				if (swing) {
					beingUsed.push_back(new Weapon(items[selectedSlot].holding, true, attackAngle, valueFromCharge()));
					animation = true;
				}
				else {
					beingUsed.push_back(new Weapon(items[selectedSlot].holding, false, attackAngle, valueFromCharge()));
					items[selectedSlot].holding = nullptr;
				}
			}
			swing = false;
			yeet = false;
			charge = 0;
		}
	}

	for (int x = 0; x < beingUsed.size(); x++) {
		Weapon* bu = beingUsed.at(x);
		// cout << "drawign\n";
		bool alive = bu->draw(window, world, entities);
		if (!alive) {
			if (bu->melee) {
				animation = false;
				swing = false;
			}

			delete bu;
			beingUsed.erase(beingUsed.begin() + x);
			x--;
		}
	}

	/*
	window->setColor(255, 0, 0, 255);
	window->cross(x, y);
	window->cross(x + show_width, y);
	window->cross(x + show_width, y + show_height);
	window->cross(x, y + show_height);
	*/

	return false;
}