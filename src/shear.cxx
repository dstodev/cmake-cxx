#include "shear.hxx"

using namespace Eigen;

void shear(Vector2d& v, float x_magnitude, float y_magnitude)
{
	static Matrix2d shear;

	shear.setIdentity();
	shear(0, 1) = x_magnitude;
	shear(1, 0) = y_magnitude;

	v = shear * v;
}
