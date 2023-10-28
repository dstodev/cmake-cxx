#ifndef ENTITY_HXX
#define ENTITY_HXX

#include <dll-export.h>
#include <point.hxx>

namespace project {

class DLL Entity
{
public:
	Entity();
	virtual ~Entity();

	auto position() const -> Point<float> const&;
	auto position() -> Point<float>&;

	bool operator==(Entity const& rhs) const = default;

protected:
	Point<float> _position;
};

}  // namespace project

#endif  // ENTITY_HXX
