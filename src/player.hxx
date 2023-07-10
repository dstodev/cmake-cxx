#ifndef PLAYER_HXX
#define PLAYER_HXX

#include <point_t.hxx>

#include <project-api.h>

namespace project {

class PROJECT_API Player
{
public:
	static int constexpr base_speed_pps = 200;  ///< pps = pixels per second

	Player();

	[[nodiscard]]
	auto position() const -> point_t<float> const&;
	auto position() -> point_t<float>&;

	bool operator==(Player const&) const = default;

private:
	point_t<float> _position;
};

}  // namespace project

#endif  // PLAYER_HXX
