#include "Item.hpp"
#include "Slot.hpp"

Item::Item(ItemData* id) {
	itemData = id;

	texture = id->texture;
	fullPicSize();
	setRect();
}

void Item::slotDraw(RenderWindow* window, Slot* s) {
	int sw = show_width;
	int sh = show_height;
	int sx = x;
	int sy = y;

	show_width = s->show_width * SLOTSCALER;
	show_height = s->show_height * SLOTSCALER;
	x = s->x + (s->show_width - show_width) / 2;
	y = s->y + (s->show_height - show_height) / 2;

	window->render(this, true);

	show_width = sw;
	show_height = sh;
	x = sx;
	y = sy;
}