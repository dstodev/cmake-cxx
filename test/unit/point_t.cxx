#include <gtest/gtest.h>

#include <point_t.hxx>

using namespace project;

TEST(Point, construct)
{
	point_t point;
	ASSERT_EQ(0, point.x());
	ASSERT_EQ(0, point.y());
}

TEST(Point, construct_with_values)
{
	point_t point {1, 2};
	ASSERT_EQ(1, point.x());
	ASSERT_EQ(2, point.y());
}

TEST(Point, assign_fields)
{
	point_t point;
	point.x() = 1;
	point.y() = 2;
	ASSERT_EQ(1, point.x());
	ASSERT_EQ(2, point.y());
}

TEST(Point, iterate_semantics)
{
	point_t point {0, 1};

	// iterate over an in-place initializer list
	for (auto i {0}; auto const& it : {point.x(), point.y()}) {
		ASSERT_EQ(i++, it);
	}
}

TEST(Point, equality)
{
	point_t point_1 {1.0f, 2};
	point_t point_2 {1, 2.0f};
	ASSERT_EQ(point_1, point_2);
}

TEST(Point, inequality)
{
	point_t point_1 {1.0f, 2};
	point_t point_2 {1, 3.0f};
	ASSERT_NE(point_1, point_2);
}

TEST(Point, copy)
{
	point_t point_1 {1.0f, 2};
	point_t point_2 {point_1};
	ASSERT_EQ(point_1, point_2);
}
