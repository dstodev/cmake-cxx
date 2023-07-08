#include <gtest/gtest.h>

#include <player.hxx>

using namespace project;

TEST(Player, construct)
{
	Player player;
	(void) player;
}

TEST(Player, position)
{
	Player player;
	ASSERT_EQ(0, player.position().x());
	ASSERT_EQ(0, player.position().y());
}

TEST(Player, position_assign_fields)
{
	Player player;
	player.position().x() = 1;
	player.position().y() = 2;
	ASSERT_EQ(1, player.position().x());
	ASSERT_EQ(2, player.position().y());
}

TEST(Player, const_player_position)
{
	Player const player;
	ASSERT_EQ(0, player.position().x());
	ASSERT_EQ(0, player.position().y());
}
