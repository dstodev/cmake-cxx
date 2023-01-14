#include "point_t.hxx"

using namespace Eigen;

namespace project {

point_t& point_t::operator=(container_type const& copy)
{
	_data = copy;
	return *this;
}

bool operator==(point_t const& lhs, point_t const& rhs)
{
	return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}

std::ostream& operator<<(std::ostream& os, point_t const& point)
{
	// https://eigen.tuxfamily.org/dox/structEigen_1_1IOFormat.html
	IOFormat static const format(4, 0, ", ", "\n", "[", "]");

	auto vector = static_cast<point_t::container_type const&>(point);

	// Eigen defaults to column-major containers, so transpose to a row-major
	// vector for more compact printing.
	os << vector.transpose().format(format);

	return os;
}

point_t::operator container_type&()
{
	return _data;
}

point_t::operator container_type const&() const
{
	return _data;
}

}  // namespace project
