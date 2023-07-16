#ifndef ENTITY_HXX
#define ENTITY_HXX

#include <point_t.hxx>
#include <project-api.h>

#include <compare>

namespace project {

class PROJECT_API Entity
{
public:
	Entity();
	virtual ~Entity() = default;

	[[nodiscard]]
	auto position() const -> point_t<float> const&;
	auto position() -> point_t<float>&;

	bool operator==(Entity const& rhs) const = default;

protected:
	point_t<float> _position;
};

}  // namespace project

#endif  // ENTITY_HXX
