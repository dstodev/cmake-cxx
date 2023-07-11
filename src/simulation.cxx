#include "simulation.hxx"

#include <log.hxx>

#include <Eigen/Dense>

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

	// Add a full impulse in each direction that is pressed, then normalize the
	// vector and scale it by the number of pixels to travel per second.

	if (control.up) {
		direction.y() -= 1.0f;
	}
	if (control.down) {
		direction.y() += 1.0f;
	}
	if (control.left) {
		direction.x() -= 1.0f;
	}
	if (control.right) {
		direction.x() += 1.0f;
	}

	if (direction.any()) {
		direction.normalize();
		direction *= Player::base_speed_pps * delta_s * (control.shift ? Player::shift_multiplier : 1.0f);
		log::trace("Player move vector: ({:.2f}, {:.2f})\n", direction.x(), direction.y());
		using container_cast_t = std::remove_reference_t<decltype(_player.position())>::container_type&;
		_player.position() = static_cast<container_cast_t>(_player.position()) + direction;
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
