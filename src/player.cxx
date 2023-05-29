#include "player.hxx"

namespace project {

point_t& Player::position()
{
	return _position;
}

point_t const& Player::position() const
{
	return _position;
}

}  // namespace project
