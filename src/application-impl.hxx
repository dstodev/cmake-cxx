#ifndef APPLICATION_IMPL_HXX
#define APPLICATION_IMPL_HXX

#include <memory>
#include <vector>

#include <application-state.hxx>
#include <event-handler.hxx>
#include <iscene.hxx>
#include <project-api.h>  // TODO: Only use this when testing
#include <scene-input-injector.hxx>
#include <scene-renderer.hxx>

struct SDL_Window;
struct SDL_Renderer;

namespace project {

class ApplicationImpl
{
public:
#define ATTR PROJECT_API
	ATTR static ApplicationImpl& instance();

	ATTR void init();
	ATTR int app_main(int argc, char* argv[]);

	ATTR void tick();
	ATTR void render();
	ATTR void quit();

	[[nodiscard]]
	ATTR auto state() const -> ApplicationState const&;
	ATTR void state(ApplicationState state);
#undef ATTR

protected:
	void run_until_user_quit();
	void handle_user_input();

	ApplicationState _state;
	EventHandler _handler;
	uint64_t _last_tick_ms;

	std::vector<std::unique_ptr<IScene>> _scenes;
	IScene* _current_scene;

	SDL_Renderer* _renderer;
	SceneRenderer _scene_renderer;
	SceneInputInjector _scene_input_injector;

	SDL_Window* _window;

private:
	ApplicationImpl();
};

}  // namespace project

#endif  // APPLICATION_IMPL_HXX
