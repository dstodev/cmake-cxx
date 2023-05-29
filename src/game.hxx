#ifndef GAME_HXX
#define GAME_HXX

#include <cstdint>
#include <vector>

#include <player.hxx>
#include <project-api.h>

namespace project {

class PROJECT_API Game
{
public:
	Game();

	void tick(uint64_t delta_ms);

	struct
	{
		bool up = false;
		bool down = false;
		bool left = false;
		bool right = false;
	} control;

	Player const& player() const;
	Player& player();

protected:
	void move_player(uint64_t delta_ms);

	Player _player;
};

}  // namespace project

#endif  // GAME_HXX
