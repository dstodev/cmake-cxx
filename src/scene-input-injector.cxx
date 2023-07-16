#include "scene-input-injector.hxx"

#include <simulation.hxx>

namespace project {

SceneInputInjector::SceneInputInjector(EventHandler& handler)
    : _handler(handler)
{}

void SceneInputInjector::visit(Simulation& scene)
{
	auto& control = scene.control();
	control.up = _handler.intent_up();
	control.down = _handler.intent_down();
	control.left = _handler.intent_left();
	control.right = _handler.intent_right();
	control.shift = _handler.intent_shift();
}

}  // namespace project
