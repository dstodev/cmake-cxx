#include <gtest/gtest.h>

#include <player.hxx>

using namespace project;

TEST(Player, construct)
{
	Player o;
	(void) o;
}

TEST(Player, position)
{
	Player o;
	ASSERT_EQ(0, o.position().x());
	ASSERT_EQ(0, o.position().y());
}

TEST(Player, position_assign_fields)
{
	Player o;
	o.position().x() = 1;
	o.position().y() = 2;
	ASSERT_EQ(1, o.position().x());
	ASSERT_EQ(2, o.position().y());
}

TEST(Player, const_player_position)
{
	Player const o;
	ASSERT_EQ(0, o.position().x());
	ASSERT_EQ(0, o.position().y());
}
