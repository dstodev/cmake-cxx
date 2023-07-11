#include "application-impl.hxx"

#include <stdexcept>

#include <SDL.h>

#include <log.hxx>
#include <render-specs.hxx>
#include <texture-cache.hxx>

namespace {
constexpr int WINDOW_WIDTH = 1600;
constexpr int WINDOW_HEIGHT = 900;
}  // namespace

namespace project {

ApplicationImpl::ApplicationImpl()
    : _state(ApplicationState::NOT_INITIALIZED)
    , _simulation(WINDOW_WIDTH, WINDOW_HEIGHT)
    , _last_tick_ms()
    , _renderer(nullptr)
    , _window(nullptr)
{}

void ApplicationImpl::init()
{
	if (_state != ApplicationState::NOT_INITIALIZED) {
		return;
	}

	if (int status; (status = SDL_Init(SDL_INIT_VIDEO)) < 0) {
		log::error("SDL_Init() returned {} because: {}\n", status, SDL_GetError());
		throw std::runtime_error("Application failed to initialize!");
	}

	// TODO: This could be fun to set to "0"
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
		log::warn("SDL_SetHint() failed!\n");
	}

	if ((_window = SDL_CreateWindow("Project",
	                                SDL_WINDOWPOS_UNDEFINED,
	                                SDL_WINDOWPOS_UNDEFINED,
	                                WINDOW_WIDTH,
	                                WINDOW_HEIGHT,
	                                SDL_WINDOW_SHOWN))
	    == nullptr) {
		log::error("SDL_CreateWindow() failed because: {}\n", SDL_GetError());
		throw std::runtime_error("Application failed to create window!");
	}

	if ((_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC))
	    == nullptr) {
		log::error("SDL_CreateRenderer() failed because: {}\n", SDL_GetError());
		throw std::runtime_error("Application failed to create renderer!");
	}

	textures::init_all(_renderer);

	_state = ApplicationState::INITIALIZED;
}

int ApplicationImpl::app_main(int argc, char* argv[])
{
	init();
	_state = ApplicationState::RUNNING;
	run_until_user_quit();
	quit();
	return 0;
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
	static struct
	{
		bool escape = false;
		bool up = false;
		bool w = false;
		bool down = false;
		bool s = false;
		bool left = false;
		bool a = false;
		bool right = false;
		bool d = false;
		bool lshift = false;
		bool rshift = false;
	} UserInput;

	auto& control = _simulation.control;
	SDL_Event event;

	while (SDL_PollEvent(&event) != 0) {
		if (event.type == SDL_QUIT) {
			_state = ApplicationState::QUITTING;
			return;
		}
		else if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {
			switch (event.key.keysym.scancode) {
			case SDL_SCANCODE_ESCAPE: UserInput.escape = true; break;
			case SDL_SCANCODE_UP: UserInput.up = true; break;
			case SDL_SCANCODE_W: UserInput.w = true; break;
			case SDL_SCANCODE_DOWN: UserInput.down = true; break;
			case SDL_SCANCODE_S: UserInput.s = true; break;
			case SDL_SCANCODE_LEFT: UserInput.left = true; break;
			case SDL_SCANCODE_A: UserInput.a = true; break;
			case SDL_SCANCODE_RIGHT: UserInput.right = true; break;
			case SDL_SCANCODE_D: UserInput.d = true; break;
			case SDL_SCANCODE_LSHIFT: UserInput.lshift = true; break;
			case SDL_SCANCODE_RSHIFT: UserInput.rshift = true; break;
			default: break;
			}
		}
		else if (event.type == SDL_KEYUP) {
			switch (event.key.keysym.scancode) {
			case SDL_SCANCODE_ESCAPE: UserInput.escape = false; break;
			case SDL_SCANCODE_UP: UserInput.up = false; break;
			case SDL_SCANCODE_W: UserInput.w = false; break;
			case SDL_SCANCODE_DOWN: UserInput.down = false; break;
			case SDL_SCANCODE_S: UserInput.s = false; break;
			case SDL_SCANCODE_LEFT: UserInput.left = false; break;
			case SDL_SCANCODE_A: UserInput.a = false; break;
			case SDL_SCANCODE_RIGHT: UserInput.right = false; break;
			case SDL_SCANCODE_D: UserInput.d = false; break;
			case SDL_SCANCODE_LSHIFT: UserInput.lshift = false; break;
			case SDL_SCANCODE_RSHIFT: UserInput.rshift = false; break;
			default: break;
			}
		}
	}

	if (UserInput.escape) {
		_state = ApplicationState::QUITTING;
	}

	control.up = UserInput.up || UserInput.w;
	control.down = UserInput.down || UserInput.s;
	control.left = UserInput.left || UserInput.a;
	control.right = UserInput.right || UserInput.d;
	control.shift = UserInput.lshift || UserInput.rshift;
}

void ApplicationImpl::tick()
{
	uint64_t current_tick_ms = SDL_GetTicks64();
	uint64_t delta_ms = current_tick_ms - _last_tick_ms;

	_last_tick_ms = current_tick_ms;
	_simulation.tick(delta_ms);
}

void ApplicationImpl::render()
{
	draw(_renderer, *this);
	draw(_renderer, _simulation);
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
