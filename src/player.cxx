#include "player.hxx"

namespace project {

Player::Player()
    : _position()
{}

auto Player::position() const -> point_t<float> const&
{
	return _position;
}

auto Player::position() -> point_t<float>&
{
	return _position;
}

}  // namespace project
