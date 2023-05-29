#include "render-specs.hxx"

#include <SDL_render.h>

#include <log.hxx>

#include <application-impl.hxx>
#include <game.hxx>
#include <player.hxx>
#include <point_t.hxx>

using namespace project;

void draw(SDL_Renderer* renderer, ApplicationImpl const& application)
{
	SDL_SetRenderDrawColor(renderer, 0xe0, 0xf0, 0xff, 0xff);
	SDL_RenderClear(renderer);
}

void draw(SDL_Renderer* renderer, Game const& game)
{
	draw(renderer, game.player());
}

void draw(SDL_Renderer* renderer, Player const& player)
{
	SDL_Texture static* player_texture = nullptr;

	if (player_texture == nullptr) {
		// Initialize the player texture
		player_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 9, 9);

		if (player_texture == nullptr) {
			log::warn("Failed to create player texture because: %s\n", SDL_GetError());
			return;
		}

		SDL_SetRenderTarget(renderer, player_texture);

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

	int width, height;
	SDL_GetRendererOutputSize(renderer, &width, &height);

	SDL_Rect center {width / 2 - 4, height / 2 - 4, 9, 9};
	SDL_RenderCopy(renderer, player_texture, nullptr, &center);

	auto position = player.position();
	position.x() += static_cast<float>(width) / 2;
	position.y() += static_cast<float>(height) / 2;

	draw(renderer, position);
}

void draw(SDL_Renderer* renderer, point_t const& point)
{
	SDL_SetRenderDrawColor(renderer, 0xff, 0x00, 0x00, 0xff);
	SDL_Rect square {static_cast<int>(point.x() - 1), static_cast<int>(point.y() - 1), 3, 3};

	if (SDL_RenderDrawRect(renderer, &square) < 0) {
		log::warn("Failed to draw type point_t because: %s\n", SDL_GetError());
	}
}
