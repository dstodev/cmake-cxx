#include "render-specs.hxx"

#include <SDL_render.h>

#include <log.hxx>

#include <application-impl.hxx>
#include <game.hxx>
#include <player.hxx>
#include <point_t.hxx>
#include <textures/textures.hxx>

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
	int width, height;
	SDL_GetRendererOutputSize(renderer, &width, &height);

	SDL_Rect center {width / 2 - 4, height / 2 - 4, 9, 9};
	SDL_RenderCopy(renderer, textures::player, nullptr, &center);

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
