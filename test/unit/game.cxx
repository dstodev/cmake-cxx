#include <gtest/gtest.h>

#include <game.hxx>
#include <player.hxx>

using namespace project;

TEST(Game, construct)
{
	Game game;
	(void) game;
}

TEST(Game, player)
{
	Game game;
	ASSERT_EQ(Player(), game.player());
}

TEST(Game, player_moves)
{
	Game game;
	game.tick(1000);
	ASSERT_EQ(Player(), game.player());

	game.control.up = true;
	game.tick(1000);
	ASSERT_NE(Player(), game.player());
}

struct PlayerMoveTestParams
{
	PlayerMoveTestParams(int delta_ms, bool up, bool down, bool left, bool right, point_t const& expected)
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
	point_t expected;
};

class PlayerMoveTests : public ::testing::TestWithParam<PlayerMoveTestParams>
{};

TEST_P(PlayerMoveTests, player_moves_adjusted_for_time)
{
	Game game;
	game.control.up = GetParam().up;
	game.control.down = GetParam().down;
	game.control.left = GetParam().left;
	game.control.right = GetParam().right;
	game.tick(GetParam().delta_ms);
	ASSERT_EQ(GetParam().expected, game.player().position());
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
        PlayerMoveTestParams(1000, true, false, true, false, point_t {-141.421356237f, -141.421356237})  // up + left
        ));
