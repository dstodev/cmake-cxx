#ifndef POINT_T_HXX
#define POINT_T_HXX

#include <ostream>

#include <Eigen/Dense>

namespace project {

template <typename T>
class point_t;

template <typename T>
bool operator==(point_t<T> const& lhs, point_t<T> const& rhs);

template <typename T>
std::ostream& operator<<(std::ostream& os, point_t<T> const& point);

template <typename T>
class point_t
{
public:
	using value_type = T;
	using container_type = Eigen::Vector2<value_type>;

	explicit point_t(value_type value_x = {}, value_type value_y = {});
	virtual ~point_t() = default;

	auto x() const -> value_type const&;
	auto x() -> value_type&;

	auto y() const -> value_type const&;
	auto y() -> value_type&;

	point_t& operator=(container_type const& copy);

	friend bool operator==<>(point_t const& lhs, point_t const& rhs);
	friend std::ostream& operator<< <>(std::ostream& os, point_t<value_type> const& point);

	operator container_type const&() const;
	operator container_type&();

private:
	container_type _data;
};

template <typename T>
point_t<T>::point_t(value_type value_x, value_type value_y)
    : _data {value_x, value_y}
{}

template <typename T>
auto point_t<T>::x() const -> value_type const&
{
	return _data[0];
}

template <typename T>
auto point_t<T>::x() -> value_type&
{
	return _data[0];
}

template <typename T>
auto point_t<T>::y() const -> value_type const&
{
	return _data[1];
}

template <typename T>
auto point_t<T>::y() -> value_type&
{
	return _data[1];
}

template <typename T>
auto point_t<T>::operator=(container_type const& copy) -> point_t&
{
	_data = copy;
	return *this;
}

template <typename T>
point_t<T>::operator container_type const&() const
{
	return _data;
}

template <typename T>
point_t<T>::operator container_type&()
{
	return _data;
}

template <typename T>
bool operator==(point_t<T> const& lhs, point_t<T> const& rhs)
{
	return lhs._data == rhs._data;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, point_t<T> const& point)
{
	// https://eigen.tuxfamily.org/dox/structEigen_1_1IOFormat.html
	static Eigen::IOFormat const format(4, 0, ", ", "\n", "[", "]");

	auto vector = static_cast<project::point_t<T>::container_type const&>(point);

	// Eigen defaults to column-major containers, so transpose to a row-major
	// vector for more-compact printing.
	os << vector.transpose().format(format);
	return os;
}

}  // namespace project

#endif  // POINT_T_HXX
