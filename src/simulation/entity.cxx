#include "entity.hxx"

namespace project {

Entity::Entity()
    : _position()
{}

auto Entity::position() const -> point_t<float> const&
{
	return _position;
}

auto Entity::position() -> point_t<float>&
{
	return _position;
}

}  // namespace project
