#ifndef APPLICATION_IMPL_HXX
#define APPLICATION_IMPL_HXX

#include <memory>
#include <vector>

#include <application-state.hxx>
#include <event-handler.hxx>
#include <project-api.h>  // TODO: Only use this when testing
#include <renderer.hxx>
#include <scene-input-visitor.hxx>
#include <scene-render-visitor.hxx>
#include <scene.hxx>

struct SDL_Window;

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
	ATTR void reset();

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

	std::vector<std::unique_ptr<Scene>> _scenes;
	Scene* _current_scene;

	Renderer _renderer;
	SceneInputVisitor _scene_input_visitor;
	SceneRenderVisitor _scene_render_visitor;

	SDL_Window* _window;

private:
	ApplicationImpl();
};

}  // namespace project

#endif  // APPLICATION_IMPL_HXX
