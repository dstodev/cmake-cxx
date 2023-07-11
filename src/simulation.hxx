#ifndef SIMULATION_HXX
#define SIMULATION_HXX

#include <cstdint>
#include <vector>

#include <player.hxx>
#include <project-api.h>

namespace project {

class PROJECT_API Simulation
{
public:
	explicit Simulation(int width = 640, int height = 480);

	void tick(uint64_t delta_ms);

	struct
	{
		bool up = false;
		bool down = false;
		bool left = false;
		bool right = false;
	} control;

	[[nodiscard]]
	int width() const;

	[[nodiscard]]
	int height() const;

	[[nodiscard]]
	auto player() const -> Player const&;
	auto player() -> Player&;

protected:
	void move_player(uint64_t delta_ms);

	int _width;
	int _height;

	Player _player;
};

}  // namespace project

#endif  // SIMULATION_HXX
