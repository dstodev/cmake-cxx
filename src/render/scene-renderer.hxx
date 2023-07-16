#ifndef SCENE_RENDERER_HXX
#define SCENE_RENDERER_HXX

#include <event-handler.hxx>
#include <scene-visitor.hxx>

struct SDL_Renderer;

namespace project {

class SceneRenderer : public SceneVisitor
{
public:
	explicit SceneRenderer(EventHandler& handler);
	~SceneRenderer() override = default;

	void visit(Simulation& scene) override;

	void renderer(SDL_Renderer* renderer);

private:
	EventHandler& _handler;
	SDL_Renderer* _renderer;
};

}  // namespace project

#endif  // SCENE_RENDERER_HXX
