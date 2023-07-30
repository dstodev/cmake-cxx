#ifndef PLAYER_HXX
#define PLAYER_HXX

#include <entity.hxx>

#include <dll-export.h>

namespace project {

class DLL Player : public Entity
{
public:
	static int constexpr base_speed_pps = 200;  ///< pps = pixels per second
	static float constexpr shift_multiplier = 1.6f;

	bool operator==(Player const& rhs) const = default;
};

}  // namespace project

#endif  // PLAYER_HXX
