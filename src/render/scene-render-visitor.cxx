#include "scene-render-visitor.hxx"

#include <renderer.hxx>
#include <simulation.hxx>

namespace project {

SceneRenderVisitor::SceneRenderVisitor(Renderer& renderer)
    : _renderer(renderer)
{}

void SceneRenderVisitor::visit(Simulation& scene)
{
	auto const& scene_const = scene;
	auto& renderer = _renderer.get();
	renderer.draw(scene_const);
}

}  // namespace project
