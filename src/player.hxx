#ifndef PLAYER_HXX
#define PLAYER_HXX

#include <point_t.hxx>

#include <project-api.h>

namespace project {

class PROJECT_API Player
{
public:
	int static constexpr base_speed_pps = 200;  // pps = pixels per second

	Player()
	    : _position()
	{}

	[[nodiscard]] point_t const& position() const;
	point_t& position();

	bool operator==(Player const&) const = default;

private:
	point_t _position;
};

}  // namespace project

#endif  // PLAYER_HXX
