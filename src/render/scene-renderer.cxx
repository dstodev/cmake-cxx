#include "scene-renderer.hxx"

#include <render-specs.hxx>

namespace project {

SceneRenderer::SceneRenderer(EventHandler& handler)
    : _handler(handler)
    , _renderer(nullptr)
{}

void SceneRenderer::visit(Simulation& scene)
{
	draw(scene, _renderer, _handler);
}

void SceneRenderer::renderer(SDL_Renderer* renderer)
{
	_renderer = renderer;
}

}  // namespace project
