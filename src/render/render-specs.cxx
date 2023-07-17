#include "render-specs.hxx"

#include <SDL_render.h>

#include <log.hxx>

#include <application-impl.hxx>
#include <player.hxx>
#include <point_t.hxx>
#include <simulation.hxx>
#include <texture-cache.hxx>

namespace project {

void draw(SDL_Renderer* renderer, Simulation const& simulation, Player const& player);
void draw(SDL_Renderer* renderer, point_t<int> const& point);

void draw(SDL_Renderer* renderer, ApplicationImpl const& application)
{
	SDL_SetRenderDrawColor(renderer, 0xe0, 0xf0, 0xff, 0xff);
	SDL_RenderClear(renderer);
}

void draw(SDL_Renderer* renderer, Simulation const& simulation, EventHandler const& handler)
{
	auto const& player = simulation.player();

	draw(renderer, simulation, player);
	draw(renderer, handler.mouse_pos());
}

void draw(SDL_Renderer* renderer, Simulation const& simulation, Player const& player)
{
	auto const& texture = textures::player;
	auto const x = simulation.width() / 2;
	auto const y = simulation.height() / 2;
	auto const rect = texture.rect_centered(x, y);

	SDL_RenderCopy(renderer, texture.data(), nullptr, &rect);

	if (simulation.control().shift) {
		auto const& texture = textures::shift;
		auto const rect = texture.rect_centered(x, y);

		SDL_RenderCopy(renderer, texture.data(), nullptr, &rect);
	}

	draw(renderer, player.position().cast<int>() + simulation.center());
}

void draw(SDL_Renderer* renderer, point_t<int> const& point)
{
	SDL_Rect const square {static_cast<int>(point.x() - 1), static_cast<int>(point.y() - 1), 3, 3};

	SDL_SetRenderDrawColor(renderer, 0xff, 0x00, 0x00, 0xff);
	SDL_RenderDrawRect(renderer, &square);
}

}  // namespace project
