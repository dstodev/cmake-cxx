#ifndef ISCENE_VISITOR_HXX
#define ISCENE_VISITOR_HXX

namespace project {

class Simulation;

class ISceneVisitor
{
public:
	virtual ~ISceneVisitor() = default;

	virtual void visit(Simulation& scene) = 0;
};

}  // namespace project

#endif  // ISCENE_VISITOR_HXX
