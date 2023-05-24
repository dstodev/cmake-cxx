#include <iostream>

#include <point_t.hxx>
#include <shear.hxx>

using namespace Eigen;
using namespace project;

int main()
{
	point_t point {1, 1};

	shear(point, 1, 0);

	std::cout << "Point after shear: " << point << std::endl;

	return 0;
}
