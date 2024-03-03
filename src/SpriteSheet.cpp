#include "SpriteSheet.hpp"

SpriteSheet::SpriteSheet() {
	
}

SpriteSheet::SpriteSheet(SDL_Texture* ptexture, int x, int y, int f) : xFrames(x), yFrames(y), frames(f) {
	texture = shared_ptr<SDL_Texture>(ptexture, sdl_deleter());
}