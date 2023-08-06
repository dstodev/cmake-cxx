#ifndef SCENE_RENDER_VISITOR_HXX
#define SCENE_RENDER_VISITOR_HXX

#include <functional>

#include <scene-visitor.hxx>

struct SDL_Renderer;

namespace project {

class Renderer;
class Simulation;

class SceneRenderVisitor : public SceneVisitor
{
public:
	explicit SceneRenderVisitor(Renderer& renderer);
	~SceneRenderVisitor() override = default;

	void visit(Simulation& scene) override;

private:
	std::reference_wrapper<Renderer> _renderer;
};

}  // namespace project

#endif  // SCENE_RENDER_VISITOR_HXX
