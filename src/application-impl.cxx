#include "application-impl.hxx"

#include <cstdlib>
#include <stdexcept>

#include <thread>

#include <SDL.h>

#include <log.hxx>
#include <reader.hxx>
#include <render-specs.hxx>
#include <state.hxx>
#include <textures.hxx>

namespace project {

ApplicationImpl::ApplicationImpl()
    : _state(ApplicationState::NOT_INITIALIZED)
    , _game()
    , _last_tick_ms()
    , _renderer(nullptr)
    , _window(nullptr)
{}

void ApplicationImpl::init()
{
	if (_state != ApplicationState::NOT_INITIALIZED) {
		return;
	}

	if (char const* level = std::getenv("LOG_LEVEL")) {
		log::set_level(log::level_from(level));
	}

	if (int status; (status = SDL_Init(SDL_INIT_VIDEO)) < 0) {
		log::error("SDL_Init() returned %d because: %s\n", status, SDL_GetError());
		throw std::runtime_error("Application failed to initialize!");
	}

	// TODO: This could be fun to set to "0"
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
		log::warn("SDL_SetHint() failed!\n");
	}

	if ((_window = SDL_CreateWindow("Project",
	                                SDL_WINDOWPOS_UNDEFINED,
	                                SDL_WINDOWPOS_UNDEFINED,
	                                640,
	                                480,
	                                SDL_WINDOW_SHOWN))
	    == nullptr) {
		log::error("SDL_CreateWindow() failed because: %s\n", SDL_GetError());
		throw std::runtime_error("Application failed to create window!");
	}

	if ((_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC))
	    == nullptr) {
		log::error("SDL_CreateRenderer() failed because: %s\n", SDL_GetError());
		throw std::runtime_error("Application failed to create renderer!");
	}

	textures::init(_renderer);

	_state = ApplicationState::INITIALIZED;

	log::error("Error messages enabled\n");
	log::warn("Warning messages enabled\n");
	log::info("Info messages enabled\n");
	log::debug("Debug messages enabled\n");
	log::trace("Trace messages enabled\n");
}

int ApplicationImpl::app_main(int argc, char* argv[])
{
	init();
	_state = ApplicationState::RUNNING;

	input::start_reader_thread();

	while (_state == ApplicationState::RUNNING) {
		auto input = input::clone();

		if (input.esc) {
			_state = ApplicationState::QUITTING;
		}

		_game.control.up = input.up;
		_game.control.down = input.down;
		_game.control.left = input.left;
		_game.control.right = input.right;

		tick();
		render();

		SDL_Delay(1);
	}

	input::stop_reader_thread();

	quit();
	return 0;
}

void ApplicationImpl::tick()
{
	uint64_t current_tick_ms = SDL_GetTicks64();
	uint64_t delta_ms = current_tick_ms - _last_tick_ms;

	_last_tick_ms = current_tick_ms;
	_game.tick(delta_ms);
}

void ApplicationImpl::render()
{
	draw(_renderer, *this);
	draw(_renderer, _game);
	SDL_RenderPresent(_renderer);
}

void ApplicationImpl::quit()
{
	if (_renderer != nullptr) {
		SDL_DestroyRenderer(_renderer);
		_renderer = nullptr;
	}

	if (_window != nullptr) {
		SDL_DestroyWindow(_window);
		_window = nullptr;
	}

	SDL_Quit();

	_state = ApplicationState::DONE;
}

ApplicationState const& ApplicationImpl::state() const
{
	return _state;
}

}  // namespace project
