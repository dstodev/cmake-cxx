#include "application-impl.hxx"

#include <stdexcept>

#include <GL/glew.h>
#include <SDL.h>

#include <log.hxx>
#include <simulation.hxx>

namespace project {

ApplicationImpl::ApplicationImpl()
    : _state(ApplicationState::NOT_INITIALIZED)
    , _handler()
    , _last_tick_ms()
    , _scenes()
    , _current_scene(nullptr)
    , _renderer(_handler)
    , _scene_input_visitor(_handler)
    , _scene_render_visitor(_renderer)
    , _window(nullptr)
{}

ApplicationImpl& ApplicationImpl::instance()
{
	static ApplicationImpl instance;
	return instance;
}

int ApplicationImpl::app_main(int argc, char* argv[])
{
	init();
	_state = ApplicationState::RUNNING;
	run_until_user_quit();
	quit();
	return 0;
}

void ApplicationImpl::init()
{
	if (_state != ApplicationState::NOT_INITIALIZED) {
		return;
	}

	_current_scene = _scenes.emplace_back(std::make_unique<Simulation>(DEFAULT_WIDTH, DEFAULT_HEIGHT)).get();

	if (int status; (status = SDL_Init(SDL_INIT_VIDEO)) < 0) {
		log::error("SDL_Init() returned {} because: {}\n", status, SDL_GetError());
		throw std::runtime_error("SDL2 failed to initialize!");
	}

	// TODO: This could be fun to set to "0"
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
		log::warn("SDL_SetHint() failed!\n");
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	auto const window_flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL;
	if ((_window = SDL_CreateWindow("Project",
	                                SDL_WINDOWPOS_CENTERED,
	                                SDL_WINDOWPOS_CENTERED,
	                                DEFAULT_WIDTH,
	                                DEFAULT_HEIGHT,
	                                window_flags))
	    == nullptr) {
		log::error("SDL_CreateWindow() failed because: {}\n", SDL_GetError());
		throw std::runtime_error("Application failed to create window!");
	}

	_renderer.init(_window);

	_state = ApplicationState::INITIALIZED;
}

void ApplicationImpl::run_until_user_quit()
{
	while (handle_user_input(), _state == ApplicationState::RUNNING) {
		tick();
		render();
	}
}

void ApplicationImpl::handle_user_input()
{
	_handler.update_event_state();

	if (_handler.signal_quit() || _handler.intent_escape()) {
		_state = ApplicationState::QUITTING;
	}
	else {
		if (_handler.signal_refresh_render()) {
			_renderer.refresh();
		}
		if (_handler.signal_window_resized()) {
			_handler.reset();

			if (auto simulation = dynamic_cast<Simulation*>(_current_scene)) {
				int width, height;
				SDL_GetWindowSize(_window, &width, &height);
				simulation->resize(width, height);
			}
			_renderer.refresh();
		}
	}
	_current_scene->accept(_scene_input_visitor);
}

void ApplicationImpl::tick()
{
	uint64_t current_tick_ms = SDL_GetTicks64();
	uint64_t delta_ms = current_tick_ms - _last_tick_ms;

	_last_tick_ms = current_tick_ms;
	_current_scene->tick(delta_ms);
}

void ApplicationImpl::render()
{
	_renderer.clear();
	_current_scene->accept(_scene_render_visitor);
	_renderer.render();
}

void ApplicationImpl::quit()
{
	_renderer.deinit();

	if (_window != nullptr) {
		SDL_DestroyWindow(_window);
		_window = nullptr;
	}

	SDL_Quit();

	_state = ApplicationState::DONE;
}

void ApplicationImpl::reset()
{
	_state = ApplicationState::NOT_INITIALIZED;
	_handler.reset();
	_last_tick_ms = 0;
	_scenes.clear();
	_current_scene = nullptr;
	_window = nullptr;
}

ApplicationState const& ApplicationImpl::state() const
{
	return _state;
}

void ApplicationImpl::state(ApplicationState state)
{
	_state = state;
}

}  // namespace project
