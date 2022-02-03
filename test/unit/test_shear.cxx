#include <Eigen/Dense>
#include <gtest/gtest.h>

#include <shear.hxx>

using namespace Eigen;

TEST(Shear, zero_magnitudes)
{
	Vector2d point {1.0f, 2.0f};

	shear(point, 0, 0);

	const Vector2d expected {1.0f, 2.0f};

	ASSERT_EQ(expected, point);
}

TEST(Shear, x_magnitude_2)
{
	Vector2d point {1.0f, 2.0f};

	shear(point, 2, 0);

	// x' = x + shear[x] * y
	//    = 1 + 2        * 2
	//    = 5
	const Vector2d expected {5.0f, point[1]};

	ASSERT_EQ(expected, point);
}

TEST(Shear, y_magnitude_2)
{
	Vector2d point {1.0f, 2.0f};

	shear(point, 0, 2);

	// y' = y + shear[y] * x
	//    = 2 + 2        * 1
	//    = 4
	const Vector2d expected {point[0], 4.0f};

	ASSERT_EQ(expected, point);
}

TEST(Shear, both_magnitudes_2)
{
	Vector2d point {1.0f, 2.0f};

	shear(point, 2, 2);

	// x' = 5
	// y' = 4
	const Vector2d expected {5.0f, 4.0f};

	ASSERT_EQ(expected, point);
}
