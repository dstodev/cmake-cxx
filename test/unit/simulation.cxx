#include <gtest/gtest.h>

#include <player.hxx>
#include <simulation.hxx>

using namespace project;

TEST(Simulation, construct)
{
	Simulation simulation;
	(void) simulation;
}

TEST(Simulation, dimensions)
{
	Simulation simulation(100, 200);
	ASSERT_EQ(100, simulation.width());
	ASSERT_EQ(200, simulation.height());
}

TEST(Simulation, resize)
{
	Simulation simulation(0, 0);
	ASSERT_EQ(0, simulation.width());
	ASSERT_EQ(0, simulation.height());

	simulation.resize(200, 400);
	ASSERT_EQ(200, simulation.width());
	ASSERT_EQ(400, simulation.height());
}

TEST(Simulation, player)
{
	Simulation simulation;
	simulation.player().position() = point_t {0.0f, 0.0f};
	ASSERT_EQ(Player(), simulation.player());
}

TEST(Simulation, player_moves)
{
	Simulation simulation;
	simulation.player().position() = point_t {0.0f, 0.0f};
	simulation.tick(1000);
	ASSERT_EQ(Player(), simulation.player());

	simulation.control().up = true;
	simulation.tick(1000);
	ASSERT_NE(Player(), simulation.player());
}

struct PlayerMoveTestParams
{
	PlayerMoveTestParams(int delta_ms,
	                     bool up,
	                     bool down,
	                     bool left,
	                     bool right,
	                     bool shift,
	                     point_t<float> const& expected)
	    : delta_ms(delta_ms)
	    , up(up)
	    , down(down)
	    , left(left)
	    , right(right)
	    , shift(shift)
	    , expected(expected)
	{}

	int delta_ms;
	bool up;
	bool down;
	bool left;
	bool right;
	bool shift;
	point_t<float> expected;
};

class PlayerMoveTests : public ::testing::TestWithParam<PlayerMoveTestParams>
{};

TEST_P(PlayerMoveTests, player_moves_adjusted_for_time)
{
	Simulation simulation;
	auto& control = simulation.control();
	simulation.player().position() = point_t {0.0f, 0.0f};
	control.up = GetParam().up;
	control.down = GetParam().down;
	control.left = GetParam().left;
	control.right = GetParam().right;
	control.shift = GetParam().shift;
	simulation.tick(GetParam().delta_ms);
	auto const& expected = GetParam().expected;
	auto const& actual = simulation.player().position();
	ASSERT_EQ(expected, actual) << "expected (x=" << expected.x() << ", y=" << expected.y()
	                            << ") but got (x=" << actual.x() << ", y=" << actual.y() << ")";
}

INSTANTIATE_TEST_SUITE_P(
    AdjustForTime,
    PlayerMoveTests,
    ::testing::Values(
        PlayerMoveTestParams(1000, true, false, false, false, false, point_t {0.0f, -1.0f * Player::base_speed_pps}),
        PlayerMoveTestParams(500, true, false, false, false, false, point_t {0.0f, -0.5f * Player::base_speed_pps}),
        PlayerMoveTestParams(0, true, false, false, false, false, point_t {0.0f, 0.0f})));

// clang-format off
INSTANTIATE_TEST_SUITE_P(AdjustForDirection, PlayerMoveTests, ::testing::Values(
    // up
    PlayerMoveTestParams(1000, true, false, false, false, false,
                         point_t {0.0f, -1.0f * Player::base_speed_pps}),

    // up + down
    PlayerMoveTestParams(1000, true, true, false, false, false,
                         point_t {0.0f, 0.0f}),

    // up + down + left
    PlayerMoveTestParams(1000, true, true, true, false, false,
                         point_t {-1.0f * Player::base_speed_pps, 0.0f}),

    // up + down + left + right
    PlayerMoveTestParams(1000, true, true, true, true, false,
                         point_t {0.0f, 0.0f}),

    // up + left
    // cos(45deg) = sin(45deg) = 0.70710678118
    PlayerMoveTestParams(1000, true, false, true, false, false,
                         point_t {-0.70710678118f * Player::base_speed_pps,
                                  -0.70710678118f * Player::base_speed_pps}),

    // up + shift
    PlayerMoveTestParams(1000, true, false, false, false, true,
                         point_t {0.0f, -1.0f * Player::base_speed_pps
                                              * Player::shift_multiplier})
));
// clang-format on
