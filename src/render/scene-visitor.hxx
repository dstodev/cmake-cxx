#ifndef SCENE_VISITOR_HXX
#define SCENE_VISITOR_HXX

struct SDL_Renderer;

namespace project {

class Simulation;

class SceneVisitor
{
public:
	SceneVisitor(SDL_Renderer* renderer);
	virtual ~SceneVisitor() = default;

	void visit(Simulation const& scene) const;

private:
	SDL_Renderer* _renderer;
};

}  // namespace project

#endif  // SCENE_VISITOR_HXX
