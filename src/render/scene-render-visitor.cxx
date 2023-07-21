#include "scene-render-visitor.hxx"

#include <renderer.hxx>
#include <simulation.hxx>

namespace project {

SceneRenderVisitor::SceneRenderVisitor(Renderer& renderer)
    : _renderer(renderer)
{}

void SceneRenderVisitor::visit(Simulation& scene)
{
	_renderer.draw(scene);
}

}  // namespace project
