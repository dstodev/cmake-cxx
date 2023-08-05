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
	auto& handler = _handler.get();
	control.up = handler.intent_up();
	control.down = handler.intent_down();
	control.left = handler.intent_left();
	control.right = handler.intent_right();
	control.shift = handler.intent_shift();
}

}  // namespace project
