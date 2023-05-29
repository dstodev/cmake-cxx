#include "application-impl.hxx"

#include <cstdlib>
#include <stdexcept>

#include <SDL.h>

#include <log.hxx>
#include <render-specs.hxx>

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
		if (!set_log_level(level)) {
			log::warn("Unrecognized log level: %s. Supported options are: error warn info debug trace\n", level);
		}
	}

	if (int status; (status = SDL_Init(SDL_INIT_VIDEO)) < 0) {
		log::error("SDL_Init() failed with status: %d, because: %s\n", status, SDL_GetError());
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

	_state = ApplicationState::INITIALIZED;

	log::error("Error messages enabled\n");
	log::warn("Warning messages enabled\n");
	log::info("Info messages enabled\n");
	log::debug("Debug messages enabled\n");
	log::trace("Trace messages enabled\n");
}

bool ApplicationImpl::set_log_level(char const* level)
{
	bool success = true;

	if (std::strcmp(level, "error") == 0) {
		SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR);
	}
	else if (std::strcmp(level, "warn") == 0) {
		SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN);
	}
	else if (std::strcmp(level, "info") == 0) {
		SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);
	}
	else if (std::strcmp(level, "debug") == 0) {
		SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_DEBUG);
	}
	else if (std::strcmp(level, "trace") == 0) {
		SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_VERBOSE);
	}
	else {
		success = false;
	}

	return success;
}

int ApplicationImpl::app_main(int argc, char* argv[])
{
	init();
	_state = ApplicationState::RUNNING;

	while (_state == ApplicationState::RUNNING) {
		SDL_Event event;

		while (SDL_PollEvent(&event) != 0) {
			if (event.type == SDL_QUIT) {
				_state = ApplicationState::QUITTING;
			}
			else if (event.type == SDL_KEYDOWN) {
				if (event.key.repeat != 0) {
					continue;
				}
				switch (event.key.keysym.scancode) {
				case SDL_SCANCODE_ESCAPE: _state = ApplicationState::QUITTING; break;
				case SDL_SCANCODE_UP:
				case SDL_SCANCODE_W: _game.control.up = true; break;
				case SDL_SCANCODE_DOWN:
				case SDL_SCANCODE_S: _game.control.down = true; break;
				case SDL_SCANCODE_LEFT:
				case SDL_SCANCODE_A: _game.control.left = true; break;
				case SDL_SCANCODE_RIGHT:
				case SDL_SCANCODE_D: _game.control.right = true; break;
				default: break;
				}
			}
			else if (event.type == SDL_KEYUP) {
				switch (event.key.keysym.scancode) {
				case SDL_SCANCODE_UP:
				case SDL_SCANCODE_W: _game.control.up = false; break;
				case SDL_SCANCODE_DOWN:
				case SDL_SCANCODE_S: _game.control.down = false; break;
				case SDL_SCANCODE_LEFT:
				case SDL_SCANCODE_A: _game.control.left = false; break;
				case SDL_SCANCODE_RIGHT:
				case SDL_SCANCODE_D: _game.control.right = false; break;
				default: break;
				}
			}
		}

		tick();
		render();

		SDL_Delay(1);
	}

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

ApplicationState& ApplicationImpl::state()
{
	return _state;
}

}  // namespace project
