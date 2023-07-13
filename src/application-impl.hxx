#ifndef APPLICATION_IMPL_HXX
#define APPLICATION_IMPL_HXX

#include <application-state.hxx>
#include <project-api.h>  // TODO: Only use this when testing
#include <simulation.hxx>

struct SDL_Window;
struct SDL_Renderer;

namespace project {

class PROJECT_API ApplicationImpl
{
public:
	static ApplicationImpl& instance();

	void init();
	int app_main(int argc, char* argv[]);

	void tick();
	void render();
	void quit();

	[[nodiscard]]
	auto state() const -> ApplicationState const&;
	void state(ApplicationState state);

protected:
	void run_until_user_quit();
	void handle_user_input();

	ApplicationState _state;
	Simulation _simulation;
	uint64_t _last_tick_ms;

	SDL_Renderer* _renderer;
	SDL_Window* _window;

private:
	ApplicationImpl();
};

}  // namespace project

#endif  // APPLICATION_IMPL_HXX
