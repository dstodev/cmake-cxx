#include <gtest/gtest.h>

#include <entity.hxx>

using namespace project;

TEST(Entity, construct)
{
	Entity entity;
	(void) entity;
}

TEST(Entity, position)
{
	Entity entity;
	ASSERT_EQ(0, entity.position().x());
	ASSERT_EQ(0, entity.position().y());
}

TEST(Entity, position_assign_fields)
{
	Entity entity;
	entity.position().x() = 1;
	entity.position().y() = 2;
	ASSERT_EQ(1, entity.position().x());
	ASSERT_EQ(2, entity.position().y());
}

TEST(Entity, const_entity_position)
{
	Entity const entity;
	ASSERT_EQ(0, entity.position().x());
	ASSERT_EQ(0, entity.position().y());
}
