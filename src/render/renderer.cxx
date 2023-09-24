#include "renderer.hxx"

#include <cmath>
#include <filesystem>

#include <Eigen/Dense>
#include <SDL.h>

#ifdef _WIN32
#include <Windows.h>
#endif
#include <GL/glew.h>
// Must include GL/glew.h before GL/gl.h
#include <GL/gl.h>

#include <event-handler.hxx>
#include <log.hxx>
#include <player.hxx>
#include <point.hxx>
#include <simulation.hxx>
#include <this-file.hxx>

namespace project {

Renderer::Renderer(EventHandler const& handler)
    : _handler(handler)
    , _window(nullptr)
    , _context(nullptr)
    , _shader_programs()
    , _current_shader_program(nullptr)
    , _as_square()
{}

Renderer::~Renderer()
{
	deinit();
}

void Renderer::init(SDL_Window* window)
{
	_window = window;
	_context = SDL_GL_CreateContext(_window);

	if (glewInit() != GLEW_OK) {
		log::error("Failed to initialize GLEW!\n");
		throw std::runtime_error("GLEW failed to initialize!");
	}

	refresh();
	compile_shaders();

	_current_shader_program = &_shader_programs.at("xr-yg-zb");
	_current_shader_program->use();

	_as_square.init();
}

void Renderer::compile_shaders()
{
	auto const shader_dir = THIS_DIRECTORY() / "shader";

	for (auto const& shader_file : std::filesystem::directory_iterator(shader_dir)) {
		auto const& shader_path = shader_file.path();
		auto const& shader_name = shader_path.stem().string();

		auto const& [iterator, inserted] = _shader_programs.try_emplace(shader_name, shader_path, shader_name);

		if (!inserted) {
			auto& key_or_value = *iterator;
			auto& program = key_or_value.second;

			program.add_shader(shader_path);
		}
	}

	for (auto& [key, program] : _shader_programs) {
		program.link();
	}
}

void Renderer::deinit()
{
	SDL_GL_DeleteContext(_context);
}

void Renderer::refresh()
{
	int window_width;
	int window_height;
	SDL_GetWindowSize(_window, &window_width, &window_height);
	glViewport(0, 0, window_width, window_height);
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
	full_square *= 0.9;

	auto blue = (sin(static_cast<float>(SDL_GetTicks64()) / 2000.0f) + 1) / 2.0f;
	auto blue_uniform = _current_shader_program->get_uniform_location("blue");
	glUniform1f(blue_uniform, blue);

	_as_square.draw(full_square.data(), GL_DYNAMIC_DRAW);
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
