#include "entity.hxx"

namespace project {

Entity::Entity()
    : _position()
{}

auto Entity::position() const -> Point<float> const&
{
	return _position;
}

auto Entity::position() -> Point<float>&
{
	return _position;
}

}  // namespace project
