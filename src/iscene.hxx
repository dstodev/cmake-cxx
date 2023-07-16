#ifndef ISCENE_HXX
#define ISCENE_HXX

#include <cstdint>

namespace project {

class ISceneVisitor;

class IScene
{
public:
	virtual ~IScene() = default;

	virtual void tick(uint64_t delta_ms) = 0;
	virtual void accept(ISceneVisitor& visitor) = 0;
};

}  // namespace project

#endif  // ISCENE_HXX
