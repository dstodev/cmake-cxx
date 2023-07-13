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
