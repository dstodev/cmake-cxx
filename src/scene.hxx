#ifndef SCENE_HXX
#define SCENE_HXX

#include <cstdint>

namespace project {

class SceneVisitor;

class Scene
{
public:
	virtual ~Scene() = default;

	virtual void tick(uint64_t delta_ms) = 0;
	virtual void accept(SceneVisitor& visitor) = 0;
};

}  // namespace project

#endif  // SCENE_HXX
