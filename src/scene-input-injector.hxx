#ifndef SCENE_INPUT_INJECTOR_HXX
#define SCENE_INPUT_INJECTOR_HXX

#include <event-handler.hxx>
#include <iscene-visitor.hxx>
#include <simulation.hxx>

namespace project {

class SceneInputInjector : public ISceneVisitor
{
public:
	explicit SceneInputInjector(EventHandler& handler);
	~SceneInputInjector() override = default;

	void visit(Simulation& scene) override;

private:
	EventHandler& _handler;
};

}  // namespace project

#endif  // SCENE_INPUT_INJECTOR_HXX
