#ifndef ENTITY_HXX
#define ENTITY_HXX

#include <dll-export.h>
#include <point_t.hxx>

namespace project {

class DLL Entity
{
public:
	Entity();
	virtual ~Entity() = default;

	auto position() const -> point_t<float> const&;
	auto position() -> point_t<float>&;

	bool operator==(Entity const& rhs) const = default;

protected:
	point_t<float> _position;
};

}  // namespace project

#endif  // ENTITY_HXX
