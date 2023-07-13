#ifndef ISCENE_HXX
#define ISCENE_HXX

#include <cstdint>

namespace project {

class SceneVisitor;

class IScene
{
public:
	virtual ~IScene() = default;

	virtual void tick(uint64_t delta_ms) = 0;
	virtual void accept(SceneVisitor const& visitor) const = 0;
};

}  // namespace project

#endif  // ISCENE_HXX
