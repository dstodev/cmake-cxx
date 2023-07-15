#ifndef SIMULATION_HXX
#define SIMULATION_HXX

#include <iscene.hxx>
#include <player.hxx>
#include <project-api.h>

namespace project {

class PROJECT_API Simulation : public IScene
{
public:
	explicit Simulation(int view_width = 640, int view_height = 480);

	void tick(uint64_t delta_ms) override;
	void accept(SceneVisitor const& visitor) const override;

	void resize(int width, int height);

	[[nodiscard]] int width() const;
	[[nodiscard]] int height() const;

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
