#include "scene-visitor.hxx"

#include <render-specs.hxx>

namespace project {

SceneVisitor::SceneVisitor(SDL_Renderer* renderer)
    : _renderer(renderer)
{}

void SceneVisitor::visit(Simulation const& scene) const
{
	draw(_renderer, scene);
}

}  // namespace project
