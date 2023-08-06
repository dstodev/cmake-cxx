#include "renderer.hxx"

#include <SDL_render.h>

#include <event-handler.hxx>
#include <player.hxx>
#include <point_t.hxx>
#include <simulation.hxx>
#include <texture-cache.hxx>

namespace project {

Renderer::Renderer(EventHandler const& handler)
    : _handler(handler)
    , _renderer(nullptr)
{}

void Renderer::init(SDL_Renderer* renderer)
{
	_renderer = renderer;
	refresh();
}

void Renderer::deinit()
{
	SDL_DestroyRenderer(_renderer);
}

void Renderer::refresh()
{
	textures::init_all(_renderer);
}

void Renderer::render()
{
	SDL_RenderPresent(_renderer);
}

void Renderer::clear()
{
	SDL_SetRenderDrawColor(_renderer, 0xe0, 0xf0, 0xff, 0xff);
	SDL_RenderClear(_renderer);
}

void Renderer::draw(Simulation const& simulation)
{
	auto const& handler = _handler.get();
	draw(simulation.player(), simulation.center());
	draw(handler.mouse_pos());
}

void Renderer::draw(Player const& player, point_t<int> const& view_center)
{
	auto const& handler = _handler.get();
	auto const& player_texture = textures::player;
	auto rect = player_texture.rect_centered(view_center);

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

	SDL_SetTextureColorMod(player_texture.data(), color_r, color_g, color_b);
	SDL_RenderCopy(_renderer, player_texture.data(), nullptr, &rect);

	if (handler.intent_shift()) {
		auto const& shift_texture = textures::shift;
		rect = shift_texture.rect_centered(view_center);

		SDL_SetTextureColorMod(shift_texture.data(), 0xe0, 0x11, 0x5f);
		SDL_RenderCopy(_renderer, shift_texture.data(), nullptr, &rect);
	}

	draw(player.position().cast<int>() + view_center);
}

void Renderer::draw(point_t<int> const& point)
{
	SDL_Rect const square {static_cast<int>(point.x() - 1), static_cast<int>(point.y() - 1), 3, 3};

	SDL_SetRenderDrawColor(_renderer, 0xff, 0x00, 0x00, 0xff);
	SDL_RenderDrawRect(_renderer, &square);
}

}  // namespace project
