#include "scene-input-visitor.hxx"

#include <event-handler.hxx>
#include <simulation.hxx>

namespace project {

SceneInputVisitor::SceneInputVisitor(EventHandler& handler)
    : _handler(handler)
{}

void SceneInputVisitor::visit(Simulation& scene)
{
	auto& control = scene.control();
	control.up = _handler.intent_up();
	control.down = _handler.intent_down();
	control.left = _handler.intent_left();
	control.right = _handler.intent_right();
	control.shift = _handler.intent_shift();
}

}  // namespace project
