#include "shear.hxx"

#include <point.hxx>

using namespace Eigen;

namespace project {

void shear(Point<float>& point, float x_magnitude, float y_magnitude)
{
	static Matrix2f transform {
	    {1, 0},
	    {0, 1}
    };

	transform(0, 1) = x_magnitude;
	transform(1, 0) = y_magnitude;

	point = transform * static_cast<Vector2f&>(point);
}

}  // namespace project
