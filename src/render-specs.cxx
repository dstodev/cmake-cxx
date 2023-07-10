#include "render-specs.hxx"

#include <SDL_render.h>

#include <log.hxx>

#include <application-impl.hxx>
#include <player.hxx>
#include <point_t.hxx>
#include <simulation.hxx>
#include <textures/textures.hxx>

using namespace project;

void draw(SDL_Renderer* renderer, ApplicationImpl const& application)
{
	SDL_SetRenderDrawColor(renderer, 0xe0, 0xf0, 0xff, 0xff);
	SDL_RenderClear(renderer);
}

void draw(SDL_Renderer* renderer, Simulation const& simulation)
{
	draw(renderer, simulation.player());
}

void draw(SDL_Renderer* renderer, Player const& player)
{
	auto const x = static_cast<int>(player.position().x());
	auto const y = static_cast<int>(player.position().y());

	int texture_width, texture_height;  // TODO: Cache these values.
	SDL_QueryTexture(textures::player, nullptr, nullptr, &texture_width, &texture_height);

	SDL_Rect rect {x - texture_width / 2, y - texture_height / 2, texture_width, texture_height};
	SDL_RenderCopy(renderer, textures::player, nullptr, &rect);
}

void draw(SDL_Renderer* renderer, point_t<int> const& point)
{
	SDL_SetRenderDrawColor(renderer, 0xff, 0x00, 0x00, 0xff);
	SDL_Rect square {static_cast<int>(point.x() - 1), static_cast<int>(point.y() - 1), 3, 3};

	if (SDL_RenderDrawRect(renderer, &square) < 0) {
		log::warn("Failed to draw type point_t because: {}\n", SDL_GetError());
	}
}
