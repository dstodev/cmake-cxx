#include "scene-input-injector.hxx"

namespace project {

SceneInputInjector::SceneInputInjector(EventHandler& handler)
    : _handler(handler)
{}

void SceneInputInjector::visit(Simulation& scene)
{
	scene.intent.up = _handler.intent_up();
	scene.intent.down = _handler.intent_down();
	scene.intent.left = _handler.intent_left();
	scene.intent.right = _handler.intent_right();
	scene.intent.shift = _handler.intent_shift();
}

}  // namespace project
