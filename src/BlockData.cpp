#include "BlockData.hpp"

BlockData::BlockData(string n, shared_ptr<SDL_Texture> t) : name(n), texture(t), x(0), y(0) {
	SDL_QueryTexture(t.get(), NULL, NULL, &w, &h);
	permissable = false;
	front = false;
}

BlockData::BlockData(string n, shared_ptr<SDL_Texture> t, int x1, int y1, int w1, int h1, float tr, bool p, bool f) : name(n), texture(t), x(x1), y(y1), w(w1), h(h1), traction(tr), permissable(p), front(f) {
	
}

bool dataCompare(BlockData* a, BlockData* b) {
	return a->name < b->name;
}