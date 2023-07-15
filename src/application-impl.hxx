#ifndef APPLICATION_IMPL_HXX
#define APPLICATION_IMPL_HXX

#include <memory>
#include <vector>

#include <application-state.hxx>
#include <event-handler.hxx>
#include <iscene.hxx>
#include <project-api.h>  // TODO: Only use this when testing
#include <scene-visitor.hxx>

struct SDL_Window;
struct SDL_Renderer;

namespace project {

class ApplicationImpl
{
public:
	PROJECT_API static ApplicationImpl& instance();

	PROJECT_API void init();
	PROJECT_API int app_main(int argc, char* argv[]);

	PROJECT_API void tick();
	PROJECT_API void render() const;
	PROJECT_API void quit();

	[[nodiscard]]
	PROJECT_API auto state() const -> ApplicationState const&;
	PROJECT_API void state(ApplicationState state);

protected:
	void run_until_user_quit();
	void handle_user_input();

	ApplicationState _state;
	EventHandler _handler;
	uint64_t _last_tick_ms;

	std::vector<std::unique_ptr<IScene>> _scenes;
	IScene* _current_scene;

	SDL_Renderer* _renderer;
	SceneVisitor _renderer_visitor;

	SDL_Window* _window;

private:
	ApplicationImpl();
};

}  // namespace project

#endif  // APPLICATION_IMPL_HXX
