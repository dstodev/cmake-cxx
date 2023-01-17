#include <gtest/gtest.h>

#include <point_t.hxx>

using namespace project;

TEST(Point, construct)
{
	point_t o;
	ASSERT_EQ(0, o.x);
	ASSERT_EQ(0, o.y);
}

TEST(Point, construct_with_values)
{
	point_t o {1, 2};
	ASSERT_EQ(1, o.x);
	ASSERT_EQ(2, o.y);
}

TEST(Point, assign_fields)
{
	point_t o;
	o.x = 1;
	o.y = 2;
	ASSERT_EQ(1, o.x);
	ASSERT_EQ(2, o.y);
}

TEST(Point, iterate_semantics)
{
	point_t o {0, 1};

	// iterate over an in-place initializer list
	for (auto i {0}; auto const& it : {o.x, o.y}) {
		ASSERT_EQ(i++, it);
	}
}
