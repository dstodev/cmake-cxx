#include "render-specs.hxx"

#include <SDL_render.h>

#include <log.hxx>

#include <application-impl.hxx>
#include <player.hxx>
#include <point_t.hxx>
#include <simulation.hxx>
#include <texture-cache.hxx>

namespace project {

void draw(Player const& player, SDL_Renderer* renderer, Simulation const& simulation, EventHandler const& handler);
void draw(point_t<int> const& point, SDL_Renderer* renderer);

void draw(ApplicationImpl const& application, SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, 0xe0, 0xf0, 0xff, 0xff);
	SDL_RenderClear(renderer);
}

void draw(Simulation const& simulation, SDL_Renderer* renderer, EventHandler const& handler)
{
	auto const& player = simulation.player();

	draw(player, renderer, simulation, handler);
	draw(handler.mouse_pos(), renderer);
}

void draw(Player const& player, SDL_Renderer* renderer, Simulation const& simulation, EventHandler const& handler)
{
	auto const& texture = textures::player;
	auto const x = simulation.width() / 2;
	auto const y = simulation.height() / 2;
	auto const rect = texture.rect_centered(x, y);

	int color_r = 0x80;
	int color_g = 0xff;
	int color_b = 0x9f;

	if (handler.mouse_left()) {
		color_r -= 0x40;
		color_g -= 0x20;
		color_b += 0x30;
	}
	if (handler.mouse_right()) {
		color_r -= 0x40;
		color_g -= 0x20;
		color_b += 0x30;
	}

	SDL_SetTextureColorMod(texture.data(), color_r, color_g, color_b);
	SDL_RenderCopy(renderer, texture.data(), nullptr, &rect);

	if (handler.intent_shift()) {
		auto const& texture = textures::shift;
		auto const rect = texture.rect_centered(x, y);

		SDL_SetTextureColorMod(texture.data(), 0xe0, 0x11, 0x5f);
		SDL_RenderCopy(renderer, texture.data(), nullptr, &rect);
	}

	draw(player.position().cast<int>() + simulation.center(), renderer);
}

void draw(point_t<int> const& point, SDL_Renderer* renderer)
{
	SDL_Rect const square {static_cast<int>(point.x() - 1), static_cast<int>(point.y() - 1), 3, 3};

	SDL_SetRenderDrawColor(renderer, 0xff, 0x00, 0x00, 0xff);
	SDL_RenderDrawRect(renderer, &square);
}

}  // namespace project
