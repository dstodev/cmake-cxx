#ifndef SCENE_INPUT_VISITOR_HXX
#define SCENE_INPUT_VISITOR_HXX

#include <functional>

#include <scene-visitor.hxx>

namespace project {

class EventHandler;
class Simulation;

class SceneInputVisitor : public SceneVisitor
{
public:
	explicit SceneInputVisitor(EventHandler& handler);
	~SceneInputVisitor() override = default;

	void visit(Simulation& scene) override;

private:
	std::reference_wrapper<EventHandler> _handler;
};

}  // namespace project

#endif  // SCENE_INPUT_VISITOR_HXX
