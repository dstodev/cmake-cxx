#ifndef SCENE_RENDER_VISITOR_HXX
#define SCENE_RENDER_VISITOR_HXX

#include <scene-visitor.hxx>

struct SDL_Renderer;

namespace project {

struct Renderer;
struct Simulation;

class SceneRenderVisitor : public SceneVisitor
{
public:
	explicit SceneRenderVisitor(Renderer& renderer);
	~SceneRenderVisitor() override = default;

	void visit(Simulation& scene) override;

private:
	Renderer& _renderer;
};

}  // namespace project

#endif  // SCENE_RENDER_VISITOR_HXX
