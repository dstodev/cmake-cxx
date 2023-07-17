#include "simulation.hxx"

#include <Eigen/Dense>

#include <log.hxx>
#include <scene-visitor.hxx>

namespace project {

Simulation::Simulation(int view_width, int view_height)
    : _intent()
    , _width(view_width)
    , _height(view_height)
    , _player()
{
	log::debug("Simulation::Simulation(view_width={}, view_height={})\n", view_width, view_height);
}

void Simulation::tick(uint64_t delta_ms)
{
	log::trace("Simulation::tick(delta_ms={})\n", delta_ms);
	move_player(delta_ms);
}

void Simulation::accept(SceneVisitor& visitor)
{
	visitor.visit(*this);
}

void Simulation::resize(int width, int height)
{
	log::debug("Simulation::resize(width={}, height={})\n", width, height);
	_width = width;
	_height = height;
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

	if (_intent.up) {
		direction.y() -= 1.0f;
	}
	if (_intent.down) {
		direction.y() += 1.0f;
	}
	if (_intent.left) {
		direction.x() -= 1.0f;
	}
	if (_intent.right) {
		direction.x() += 1.0f;
	}

	if (direction.any()) {
		direction.normalize();
		direction *= Player::base_speed_pps * delta_s * (_intent.shift ? Player::shift_multiplier : 1.0f);
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

point_t<int> Simulation::center() const
{
	return point_t<int> {_width / 2, _height / 2};
}

auto Simulation::control() const -> const Simulation::Control&
{
	return _intent;
}

auto Simulation::control() -> Simulation::Control&
{
	return _intent;
}

}  // namespace project
