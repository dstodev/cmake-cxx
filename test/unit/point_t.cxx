#include <gtest/gtest.h>

#include <point_t.hxx>

using namespace project;

TEST(Point, construct)
{
	point_t<float> point;
	ASSERT_EQ(0, point.x());
	ASSERT_EQ(0, point.y());
}

TEST(Point, construct_with_values)
{
	point_t<float> point {1, 2};
	ASSERT_EQ(1, point.x());
	ASSERT_EQ(2, point.y());
}

TEST(Point, assign_fields)
{
	point_t<float> point;
	point.x() = 1;
	point.y() = 2;
	ASSERT_EQ(1, point.x());
	ASSERT_EQ(2, point.y());
}

TEST(Point, iterate_semantics)
{
	point_t<float> point {0, 1};

	// iterate over an in-place initializer list
	for (auto i {0}; auto const& it : {point.x(), point.y()}) {
		ASSERT_EQ(i++, it);
	}
}

TEST(Point, equality)
{
	point_t<float> point_1 {1.0f, 2};
	point_t<float> point_2 {1, 2.0f};
	ASSERT_EQ(point_1, point_2);
}

TEST(Point, inequality)
{
	point_t<float> point_1 {1.0f, 2};
	point_t<float> point_2 {1, 3.0f};
	ASSERT_NE(point_1, point_2);
}

TEST(Point, copy)
{
	point_t<float> point_1 {1.0f, 2};
	point_t<float> point_2 {point_1};
	ASSERT_EQ(point_1, point_2);
}

TEST(Point, addition)
{
	point_t<float> point_1 {1.0f, 2.0f};
	point_t<float> point_2 {3.0f, 4.0f};
	point_t<float> expected {4.0f, 6.0f};
	ASSERT_EQ(expected, point_1 + point_2);
}

TEST(Point, subtraction)
{
	point_t<float> point_1 {1.0f, 2.0f};
	point_t<float> point_2 {3.0f, 4.0f};
	point_t<float> expected {-2.0f, -2.0f};
	ASSERT_EQ(expected, point_1 - point_2);
}

TEST(Point, multiplication)
{
	point_t<float> point {1.0f, 2.0f};
	point_t<float> expected {2.0f, 4.0f};
	ASSERT_EQ(expected, point * 2.0f);
}

TEST(Point, division)
{
	point_t<float> point {1.0f, 2.0f};
	point_t<float> expected {0.5f, 1.0f};
	ASSERT_EQ(expected, point / 2.0f);
}

TEST(Point, addition_assignment)
{
	point_t<float> point_1 {1.0f, 2.0f};
	point_t<float> point_2 {3.0f, 4.0f};
	point_t<float> expected {4.0f, 6.0f};
	ASSERT_EQ(expected, point_1 += point_2);
}

TEST(Point, subtraction_assignment)
{
	point_t<float> point_1 {1.0f, 2.0f};
	point_t<float> point_2 {3.0f, 4.0f};
	point_t<float> expected {-2.0f, -2.0f};
	ASSERT_EQ(expected, point_1 -= point_2);
}

TEST(Point, multiplication_assignment)
{
	point_t<float> point {1.0f, 2.0f};
	point_t<float> expected {2.0f, 4.0f};
	ASSERT_EQ(expected, point *= 2.0f);
}

TEST(Point, division_assignment)
{
	point_t<float> point {1.0f, 2.0f};
	point_t<float> expected {0.5f, 1.0f};
	ASSERT_EQ(expected, point /= 2.0f);
}

TEST(Point, cast)
{
	point_t<float> point {1.0f, 2.0f};
	point_t<int> expected {1, 2};
	ASSERT_EQ(expected, point.cast<int>());
}
