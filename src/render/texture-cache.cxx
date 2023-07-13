#include "texture-cache.hxx"

#include <log.hxx>

namespace project::textures {

void init_all(SDL_Renderer* renderer)
{
	player.init(renderer);
	shift.init(renderer);
}

Texture player([](SDL_Renderer* renderer) {
	auto texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 9, 9);
	if (texture == nullptr) {
		log::warn("Failed to create player texture because: {}\n", SDL_GetError());
	}
	else {
		SDL_SetRenderTarget(renderer, texture);

		SDL_SetRenderDrawColor(renderer, 0x80, 0xff, 0xa0, 0xff);
		SDL_Rect square {2, 2, 5, 5};
		SDL_RenderFillRect(renderer, &square);

		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);
		square = {1, 1, 7, 7};
		SDL_RenderDrawRect(renderer, &square);

		SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
		square = {0, 0, 9, 9};
		SDL_RenderDrawRect(renderer, &square);

		SDL_SetRenderTarget(renderer, nullptr);
	}
	return texture;
});

Texture shift([](SDL_Renderer* renderer) {
	auto texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 13, 13);
	if (texture == nullptr) {
		log::warn("Failed to create shift texture because: {}\n", SDL_GetError());
	}
	else {
		SDL_SetRenderTarget(renderer, texture);
		SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

		SDL_SetRenderDrawColor(renderer, 0xe0, 0x11, 0x5f, 0xff);

		SDL_Rect square = {0, 0, 13, 13};
		SDL_RenderDrawRect(renderer, &square);

		SDL_SetRenderTarget(renderer, nullptr);
	}
	return texture;
});

}  // namespace project::textures
