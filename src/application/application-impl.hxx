#ifndef APPLICATION_IMPL_HXX
#define APPLICATION_IMPL_HXX

#include <memory>
#include <vector>

#include <application-state.hxx>
#include <dll-export.h>  // TODO: Only use this when testing
#include <event-handler.hxx>
#include <renderer.hxx>
#include <scene-input-visitor.hxx>
#include <scene-render-visitor.hxx>
#include <scene.hxx>

struct SDL_Window;

namespace project {

class DLL ApplicationImpl
{
public:
	static ApplicationImpl& instance();
	virtual ~ApplicationImpl() = default;

	ApplicationImpl(ApplicationImpl const& copy) = delete;
	ApplicationImpl(ApplicationImpl&& move) = default;

	ApplicationImpl& operator=(ApplicationImpl const& copy) = delete;
	ApplicationImpl& operator=(ApplicationImpl&& move) = default;

	void init();
	int app_main(int argc, char* argv[]);

	void tick();
	void render();
	void quit();
	void reset();

	auto state() const -> ApplicationState const&;
	void state(ApplicationState state);

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
