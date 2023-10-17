#include "renderer.hxx"

#include <Eigen/Dense>
#include <SDL.h>

#ifdef _WIN32
#include <Windows.h>
#endif
#include <GL/glew.h>

#include <attribute.hxx>
#include <event-handler.hxx>
#include <log.hxx>
#include <player.hxx>
#include <point.hxx>
#include <simulation.hxx>

using project::vao::Attribute;

namespace project {

Renderer::Renderer(EventHandler const& handler)
    : _handler(handler)
    , _window(nullptr)
    , _context(nullptr)
    , _shaders()
    , _squares()
{}

void Renderer::init(SDL_Window* window)
{
	_window = window;
	_context = SDL_GL_CreateContext(_window);

	if (glewInit() != GLEW_OK) {
		log::error("Failed to initialize GLEW!\n");
		throw std::runtime_error("GLEW failed to initialize!");
	}

	refresh();

	_shaders.compile();
	_shaders.use("xr-yg-zb");

	_squares.add_attribute({0, 3, Attribute::Type::Float, 0});

	//	// Second shader parameter is 3-float color vector
	//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));
	//	glEnableVertexAttribArray(1);

	_squares.init();
}

void Renderer::refresh()
{
	int window_width;
	int window_height;
	SDL_GetWindowSize(_window, &window_width, &window_height);
	glViewport(0, 0, window_width, window_height);
}

void Renderer::deinit()
{
	_squares.deinit();

	SDL_GL_DeleteContext(_context);
	_context = nullptr;

	_window = nullptr;
}

void Renderer::render()
{
	SDL_GL_SwapWindow(_window);
}

void Renderer::clear()
{
	glClearColor((0xe0 / 255.0f), (0xf0 / 255.0f), 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::draw(Simulation const& simulation)
{
	auto const& events = _handler.get();
	draw(simulation.player(), simulation.center());
	draw(events.mouse_pos());

	Eigen::Matrix<float, 4, 3, Eigen::RowMajor> full_square {
	    {-1.0f, 1.0f, 0.0f},  // top-left
	    {1.0f, 1.0f, 0.0f},  // top-right
	    {1.0f, -1.0f, 0.0f},  // bottom-right
	    {-1.0f, -1.0f, 0.0f}  // bottom-left
	};

	// TODO: use shaders in render objects like Square?
	//	auto const& shader = _shaders.get("xr-yg-zb");
	//	auto blue = (sin(static_cast<float>(SDL_GetTicks64()) / 2000.0f) + 1) / 2.0f;
	//	auto blue_uniform = shader.get_uniform_location("blue");
	//	glUniform1f(blue_uniform, blue);

	_squares.set_vertices(full_square.data(), 1, GL_DYNAMIC_DRAW);
	_squares.draw();
}

void Renderer::draw(Player const& player, Point<int> const& view_center)
{
	auto const& events = _handler.get();
	// auto const& player_texture = textures::player;
	// auto rect = player_texture.rect_centered(view_center);

	int color_r = 0x80;
	int color_g = 0xff;
	int color_b = 0x9f;

	if (events.mouse_left()) {
		color_r -= 0x40;
		color_g -= 0x20;
		color_b += 0x30;
	}
	if (events.mouse_right()) {
		color_r -= 0x40;
		color_g -= 0x20;
		color_b += 0x30;
	}

	// SDL_SetTextureColorMod(player_texture.data(), color_r, color_g, color_b);
	// SDL_RenderCopy(_context, player_texture.data(), nullptr, &rect);

	if (events.intent_shift()) {
		// auto const& shift_texture = textures::shift;
		// rect = shift_texture.rect_centered(view_center);

		// SDL_SetTextureColorMod(shift_texture.data(), 0xe0, 0x11, 0x5f);
		// SDL_RenderCopy(_context, shift_texture.data(), nullptr, &rect);
	}

	draw(player.position().cast<int>() + view_center);
}

void Renderer::draw(Point<int> const& point)
{
	SDL_Rect const square {static_cast<int>(point.x() - 1), static_cast<int>(point.y() - 1), 3, 3};

	// SDL_SetRenderDrawColor(_context, 0xff, 0x00, 0x00, 0xff);
	// SDL_RenderDrawRect(_context, &square);
}

}  // namespace project
