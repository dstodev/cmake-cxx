#include "game.hxx"

#include <log.hxx>

#include <Eigen/Dense>

namespace project {

Game::Game()
    : _player()
{}

void Game::tick(uint64_t delta_ms)
{
	log::trace("Game::tick(delta_ms={})\n", delta_ms);

	move_player(delta_ms);
}

void Game::move_player(uint64_t delta_ms)
{
	float const pixels_per_second = Player::base_speed_pps;
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
		direction *= pixels_per_second * delta_s;
		log::trace("Player moving: x={}, y={}\n", direction.x(), direction.y());
		_player.position() = Eigen::Vector2f(_player.position()) + direction;
	}
}

Player const& Game::player() const
{
	return _player;
}

Player& Game::player()
{
	return _player;
}

}  // namespace project
