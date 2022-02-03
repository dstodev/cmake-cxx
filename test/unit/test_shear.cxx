#include <Eigen/Dense>
#include <gtest/gtest.h>

#include <shear.hxx>

using namespace Eigen;

TEST(Shear, zero_magnitudes)
{
	Vector2d point {1.0f, 2.0f};

	const Vector2d expected {1.0f, 2.0f};

	shear(point, 0, 0);

	ASSERT_EQ(expected, point);
}

TEST(Shear, x_magnitude_2)
{
	Vector2d point {1.0f, 2.0f};

	// x' = x + shear[x] * y
	//    = 1 + 2        * 2
	//    = 5
	const Vector2d expected {5.0f, point[1]};

	shear(point, 2, 0);

	ASSERT_EQ(expected, point);
}

TEST(Shear, y_magnitude_2)
{
	Vector2d point {1.0f, 2.0f};

	// y' = y + shear[y] * x
	//    = 2 + 2        * 1
	//    = 4
	const Vector2d expected {point[0], 4.0f};

	shear(point, 0, 2);

	ASSERT_EQ(expected, point);
}

TEST(Shear, both_magnitudes_2)
{
	Vector2d point {1.0f, 2.0f};

	// x' = 5
	// y' = 4
	const Vector2d expected {5.0f, 4.0f};

	shear(point, 2, 2);

	ASSERT_EQ(expected, point);
}
