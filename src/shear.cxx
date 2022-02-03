#include "shear.hxx"

using namespace Eigen;

void shear(Vector2d& v, float x_magnitude, float y_magnitude)
{
	Matrix2d shear {{1.0f, x_magnitude}, {y_magnitude, 1.0f}};
	v = shear * v;
}
