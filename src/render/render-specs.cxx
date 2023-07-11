#include "render-specs.hxx"

#include <SDL_render.h>

#include <log.hxx>

#include <application-impl.hxx>
#include <player.hxx>
#include <point_t.hxx>
#include <simulation.hxx>
#include <texture-cache.hxx>

using namespace project;

void draw(SDL_Renderer* renderer, ApplicationImpl const& application)
{
	SDL_SetRenderDrawColor(renderer, 0xe0, 0xf0, 0xff, 0xff);
	SDL_RenderClear(renderer);
}

void draw(SDL_Renderer* renderer, Simulation const& simulation)
{
	draw(renderer, simulation.player());

	if (simulation.control.shift) {
		auto& player = simulation.player();
		auto const x = static_cast<int>(player.position().x());
		auto const y = static_cast<int>(player.position().y());
		SDL_SetRenderDrawColor(renderer, 0xe0, 0x11, 0x5f, 0xff);
		SDL_Rect square = {x - 6, y - 6, 13, 13};
		SDL_RenderDrawRect(renderer, &square);
	}
}

void draw(SDL_Renderer* renderer, Player const& player)
{
	auto const& texture = textures::player;
	auto const x = static_cast<int>(player.position().x());
	auto const y = static_cast<int>(player.position().y());

	SDL_Rect rect {x - texture.width() / 2, y - texture.height() / 2, texture.width(), texture.height()};
	SDL_RenderCopy(renderer, texture.data(), nullptr, &rect);
}

void draw(SDL_Renderer* renderer, point_t<int> const& point)
{
	SDL_SetRenderDrawColor(renderer, 0xff, 0x00, 0x00, 0xff);
	SDL_Rect square {static_cast<int>(point.x() - 1), static_cast<int>(point.y() - 1), 3, 3};

	if (SDL_RenderDrawRect(renderer, &square) < 0) {
		log::warn("Failed to draw type point_t because: {}\n", SDL_GetError());
	}
}
