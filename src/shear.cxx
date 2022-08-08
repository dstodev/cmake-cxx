#include "shear.hxx"

namespace project {

void shear(point_t& point, float x_magnitude, float y_magnitude)
{
	static Eigen::Matrix2f transform {{1, 0}, {0, 1}};

	transform(0, 1) = x_magnitude;
	transform(1, 0) = y_magnitude;

	point = transform * point;
}

}  // namespace project
