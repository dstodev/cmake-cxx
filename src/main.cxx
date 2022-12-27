#include <iostream>

#include <Eigen/Dense>

#include <shear.hxx>

using namespace Eigen;
using namespace project;

static IOFormat const verbose(FullPrecision, 0, ", ", ";\n", "[", "]", "[", "]");

int main()
{
	point_t point {1, 1};

	shear(point, 1, 0);

	std::cout << "Point after shear: " << point.transpose().format(verbose) << std::endl;

	return 0;
}
