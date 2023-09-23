#include "renderer.hxx"

#include <SDL.h>

#ifdef _WIN32
#include <Windows.h>
#endif
#include <GL/glew.h>
// Must include GL/glew.h before GL/gl.h
#include <GL/gl.h>

#include <event-handler.hxx>
#include <file-to-string.hxx>
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
	as_square.init();
}

void Renderer::compile_shaders()
{
	auto const shader_dir = THIS_DIRECTORY() / "shader";

	int success;
	char info_log[512];

	unsigned int shader_program = glCreateProgram();

	for (auto const& shader_file : std::filesystem::directory_iterator(shader_dir)) {
		auto const& shader_path = shader_file.path();
		auto const extension = shader_path.extension();
		auto const filename = shader_path.filename().string();

		unsigned int shader_type;

		if (extension == ".vs") {
			log::info("Compiling vertex shader: {}\n", filename);
			shader_type = GL_VERTEX_SHADER;
		}
		else if (extension == ".fs") {
			log::info("Compiling fragment shader: {}\n", filename);
			shader_type = GL_FRAGMENT_SHADER;
		}
		else {
			log::warn("Unknown shader file: {}\n", shader_path.string());
			continue;
		}

		// The shader source is read as one single string (with multiple newlines),
		// which is compilable by OpenGL.
		auto shader_source = file_to_string(shader_path);
		auto const* shader_source_ptr = shader_source.c_str();

		unsigned int shader_id = glCreateShader(shader_type);
		glShaderSource(shader_id, 1, &shader_source_ptr, nullptr);
		glCompileShader(shader_id);

		glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);

		if (!success) {
			glGetShaderInfoLog(shader_id, sizeof(info_log), nullptr, info_log);
			log::error("Shader compilation failed: {}\n", info_log);
		}
		else {
			log::info("Shader compilation succeeded!\n");
		}
		glAttachShader(shader_program, shader_id);
		glDeleteShader(shader_id);
	}

	glLinkProgram(shader_program);
	glGetProgramiv(shader_program, GL_LINK_STATUS, &success);

	if (success) {
		log::info("Shader linking succeeded!\n");
		glUseProgram(shader_program);
		_shader_program = shader_program;
	}
	else {
		glGetProgramInfoLog(shader_program, sizeof(info_log), nullptr, info_log);
		log::error("Shader linking failed: {}\n", info_log);
	}
}

void Renderer::deinit()
{
	glDeleteProgram(_shader_program);
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

	// clang-format off
	static float constexpr square[] = {
	    -0.5f, 0.5f, 0.0f,  // top-left
	    0.5f, 0.5f, 0.0f,   // top-right
	    0.5f, -0.5f, 0.0f,  // bottom-right
	    -0.5f, -0.5f, 0.0f  // bottom-left
	};
	// clang-format on

	as_square.draw(square, GL_DYNAMIC_DRAW);
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
