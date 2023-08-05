#ifndef SCENE_VISITOR_HXX
#define SCENE_VISITOR_HXX

namespace project {

class Simulation;

class SceneVisitor
{
public:
	virtual ~SceneVisitor() = default;

	virtual void visit(Simulation& scene) = 0;
};

}  // namespace project

#endif  // SCENE_VISITOR_HXX
