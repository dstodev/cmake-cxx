#ifndef APPLICATION_IMPL_HXX
#define APPLICATION_IMPL_HXX

#include <cstdint>

#include <application-state.hxx>
#include <game.hxx>
#include <project-api.h>  // TODO: Only use this when testing

struct SDL_Window;
struct SDL_Renderer;

namespace project {

class PROJECT_API ApplicationImpl
{
public:
	ApplicationImpl();

	void init();
	int app_main(int argc, char* argv[]);

	void tick();
	void render();
	void quit();

	ApplicationState& state();

protected:
	bool set_log_level(char const* level);

	ApplicationState _state;
	Game _game;
	uint64_t _last_tick_ms;

	SDL_Renderer* _renderer;
	SDL_Window* _window;
};

}  // namespace project

#endif  // APPLICATION_IMPL_HXX
