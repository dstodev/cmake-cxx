#include <gtest/gtest.h>

#include <player.hxx>
#include <simulation.hxx>
#include <user-input.hxx>

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

	UserInput = user_input_t {};
	UserInput.up = true;
	simulation.tick(1000);
	ASSERT_NE(Player(), simulation.player());
}

struct PlayerMoveTestParams
{
	PlayerMoveTestParams(int delta_ms, bool up, bool down, bool left, bool right, point_t<float> const& expected)
	    : delta_ms(delta_ms)
	    , up(up)
	    , down(down)
	    , left(left)
	    , right(right)
	    , expected(expected)
	{}

	int delta_ms;
	bool up;
	bool down;
	bool left;
	bool right;
	point_t<float> expected;
};

class PlayerMoveTests : public ::testing::TestWithParam<PlayerMoveTestParams>
{};

TEST_P(PlayerMoveTests, player_moves_adjusted_for_time)
{
	Simulation simulation;
	simulation.player().position() = point_t {0.0f, 0.0f};
	UserInput = user_input_t {};
	UserInput.up = GetParam().up;
	UserInput.down = GetParam().down;
	UserInput.left = GetParam().left;
	UserInput.right = GetParam().right;
	simulation.tick(GetParam().delta_ms);
	ASSERT_EQ(GetParam().expected, simulation.player().position());
}

INSTANTIATE_TEST_SUITE_P(
    AdjustForTime,
    PlayerMoveTests,
    ::testing::Values(PlayerMoveTestParams(1000, true, false, false, false, point_t {0.0f, -200.0f}),
                      PlayerMoveTestParams(500, true, false, false, false, point_t {0.0f, -100.0f}),
                      PlayerMoveTestParams(0, true, false, false, false, point_t {0.0f, 0.0f})));

INSTANTIATE_TEST_SUITE_P(
    AdjustForDirection,
    PlayerMoveTests,
    ::testing::Values(
        PlayerMoveTestParams(1000, true, false, false, false, point_t {0.0f, -200.0f}),  // up
        PlayerMoveTestParams(1000, true, true, false, false, point_t {0.0f, 0.0f}),  // up + down
        PlayerMoveTestParams(1000, true, true, true, false, point_t {-200.0f, 0.0f}),  // up + down + left
        PlayerMoveTestParams(1000, true, true, true, true, point_t {0.0f, 0.0f}),  // up + down + left + right
        PlayerMoveTestParams(1000, true, false, true, false, point_t {-141.421356237f, -141.421356237f})  // up + left
        ));
