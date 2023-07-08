#include "reader.hxx"

#include <memory>
#include <thread>

#include <SDL.h>

#include <state.hxx>

namespace project::input {

namespace {
constexpr int poll_hz = 500;
std::atomic<bool> quit {false};
std::unique_ptr<std::thread> reader_thread = nullptr;
}  // namespace

void update_virtual_input()
{
	SDL_Event event;
	State state;

	while (!quit) {
		while (SDL_PollEvent(&event) != 0) {
			if (event.type == SDL_KEYDOWN) {
				if (event.key.repeat != 0) {
					continue;
				}
				switch (event.key.keysym.scancode) {
				case SDL_SCANCODE_ESCAPE: state.esc = true; break;
				case SDL_SCANCODE_UP:
				case SDL_SCANCODE_W: state.up = true; break;
				case SDL_SCANCODE_DOWN:
				case SDL_SCANCODE_S: state.down = true; break;
				case SDL_SCANCODE_LEFT:
				case SDL_SCANCODE_A: state.left = true; break;
				case SDL_SCANCODE_RIGHT:
				case SDL_SCANCODE_D: state.right = true; break;
				default: break;
				}
			}
			else if (event.type == SDL_KEYUP) {
				switch (event.key.keysym.scancode) {
				case SDL_SCANCODE_UP:
				case SDL_SCANCODE_W: state.up = false; break;
				case SDL_SCANCODE_DOWN:
				case SDL_SCANCODE_S: state.down = false; break;
				case SDL_SCANCODE_LEFT:
				case SDL_SCANCODE_A: state.left = false; break;
				case SDL_SCANCODE_RIGHT:
				case SDL_SCANCODE_D: state.right = false; break;
				default: break;
				}
			}
		}
		update(state);
		SDL_Delay(1000 / poll_hz);
	}
}

void start_reader_thread()
{
	if (reader_thread) {
		return;
	}
	quit = false;
	reader_thread = std::make_unique<std::thread>(update_virtual_input);
}

void stop_reader_thread()
{
	if (!reader_thread) {
		return;
	}
	quit = true;
	reader_thread->join();
	reader_thread.reset();
}

}  // namespace project::input
