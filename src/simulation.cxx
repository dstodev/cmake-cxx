#include "simulation.hxx"

#include <Eigen/Dense>

#include <log.hxx>
#include <scene-visitor.hxx>
#include <user-input.hxx>

namespace project {

Simulation::Simulation(int width, int height)
    : _width(width)
    , _height(height)
    , _player()
{
	_player.position().x() = static_cast<float>(width) / 2.0f;
	_player.position().y() = static_cast<float>(height) / 2.0f;
}

void Simulation::tick(uint64_t delta_ms)
{
	log::trace("Simulation::tick(delta_ms={})\n", delta_ms);
	move_player(delta_ms);
}

void Simulation::accept(SceneVisitor const& visitor) const
{
	visitor.visit(*this);
}

int Simulation::width() const
{
	return _width;
}

int Simulation::height() const
{
	return _height;
}

void Simulation::move_player(uint64_t delta_ms)
{
	auto const delta_s = static_cast<float>(delta_ms) / 1000.0f;
	Eigen::Vector2f direction(0.0f, 0.0f);

	bool up = UserInput.up || UserInput.w;
	bool down = UserInput.down || UserInput.s;
	bool left = UserInput.left || UserInput.a;
	bool right = UserInput.right || UserInput.d;
	bool shift = UserInput.lshift || UserInput.rshift;

	// Add a full impulse in each direction that is pressed, then normalize the
	// vector and scale it by the number of pixels to travel per second.

	if (up) {
		direction.y() -= 1.0f;
	}
	if (down) {
		direction.y() += 1.0f;
	}
	if (left) {
		direction.x() -= 1.0f;
	}
	if (right) {
		direction.x() += 1.0f;
	}

	if (direction.any()) {
		direction.normalize();
		direction *= Player::base_speed_pps * delta_s * (shift ? Player::shift_multiplier : 1.0f);
		log::trace("Player move vector: ({:.2f}, {:.2f})\n", direction.x(), direction.y());
		_player.position() = static_cast<Eigen::Vector2f>(_player.position()) + direction;
	}
}

Player const& Simulation::player() const
{
	return _player;
}

Player& Simulation::player()
{
	return _player;
}

}  // namespace project
