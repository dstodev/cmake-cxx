#ifndef POINT_HXX
#define POINT_HXX

#include <ostream>

#include <Eigen/Dense>

namespace project {

template <typename T>
class Point;

template <typename T>
auto operator+=(Point<T>& lhs, Point<T> const& rhs) -> Point<T>&;

template <typename T>
auto operator-=(Point<T>& lhs, Point<T> const& rhs) -> Point<T>&;

template <typename T>
auto operator+(Point<T> lhs, Point<T> const& rhs) -> Point<T>;

template <typename T>
auto operator-(Point<T> lhs, Point<T> const& rhs) -> Point<T>;

template <typename T>
bool operator==(Point<T> const& lhs, Point<T> const& rhs);

template <typename T>
std::ostream& operator<<(std::ostream& os, Point<T> const& point);

template <typename T>
class Point
{
public:
	using value_type = T;
	using container_type = Eigen::Vector2<value_type>;

	explicit Point(value_type value_x = {}, value_type value_y = {});
	virtual ~Point();

	Point(Point const& copy);
	explicit Point(container_type const& copy);

	auto operator=(Point const& copy) -> Point&;
	auto operator=(container_type const& copy) -> Point&;

	auto x() const -> value_type const&;
	auto x() -> value_type&;

	auto y() const -> value_type const&;
	auto y() -> value_type&;

	friend auto operator+= <>(Point& lhs, Point const& rhs) -> Point&;
	friend auto operator-= <>(Point& lhs, Point const& rhs) -> Point&;

	friend auto operator+ <>(Point lhs, Point const& rhs) -> Point;
	friend auto operator- <>(Point lhs, Point const& rhs) -> Point;

	auto operator*=(value_type const& rhs) -> Point&;
	auto operator/=(value_type const& rhs) -> Point&;

	auto operator*(value_type const& rhs) -> Point;
	auto operator/(value_type const& rhs) -> Point;

	friend bool operator== <>(Point const& lhs, Point const& rhs);
	friend auto operator<< <>(std::ostream& os, Point<value_type> const& point) -> std::ostream&;

	/* explicit */ operator container_type const&() const;
	/* explicit */ operator container_type&();

	template <typename Y>
	auto cast() const -> Point<Y>;

private:
	container_type _data;
};

template <typename T>
Point<T>::Point(value_type value_x, value_type value_y)
    : _data {value_x, value_y}
{}

template <typename T>
Point<T>::Point(Point const& copy)
    : _data {copy}
{}

template <typename T>
Point<T>::Point(container_type const& copy)
    : _data {copy}
{}

template <typename T>
Point<T>::~Point() = default;

template <typename T>
auto Point<T>::operator=(Point const& copy) -> Point&
{
	_data = copy;
	return *this;
}

template <typename T>
auto Point<T>::operator=(container_type const& copy) -> Point&
{
	_data = copy;
	return *this;
}

template <typename T>
auto Point<T>::x() const -> value_type const&
{
	return _data[0];
}

template <typename T>
auto Point<T>::x() -> value_type&
{
	return _data[0];
}

template <typename T>
auto Point<T>::y() const -> value_type const&
{
	return _data[1];
}

template <typename T>
auto Point<T>::y() -> value_type&
{
	return _data[1];
}

template <typename T>
auto operator+=(Point<T>& lhs, Point<T> const& rhs) -> Point<T>&
{
	lhs._data += rhs._data;
	return lhs;
}

template <typename T>
auto operator-=(Point<T>& lhs, Point<T> const& rhs) -> Point<T>&
{
	lhs._data -= rhs._data;
	return lhs;
}

template <typename T>
auto operator+(Point<T> lhs, Point<T> const& rhs) -> Point<T>
{
	lhs += rhs;
	return lhs;
}

template <typename T>
auto operator-(Point<T> lhs, Point<T> const& rhs) -> Point<T>
{
	lhs -= rhs;
	return lhs;
}

template <typename T>
auto Point<T>::operator*=(value_type const& rhs) -> Point<T>&
{
	_data *= rhs;
	return *this;
}

template <typename T>
auto Point<T>::operator/=(value_type const& rhs) -> Point<T>&
{
	_data /= rhs;
	return *this;
}

template <typename T>
auto Point<T>::operator*(value_type const& rhs) -> Point<T>
{
	auto result = *this;
	result *= rhs;
	return result;
}

template <typename T>
auto Point<T>::operator/(value_type const& rhs) -> Point<T>
{
	auto result = *this;
	result /= rhs;
	return result;
}

template <typename T>
Point<T>::operator container_type const&() const
{
	return _data;
}

template <typename T>
Point<T>::operator container_type&()
{
	return _data;
}

template <typename T>
bool operator==(Point<T> const& lhs, Point<T> const& rhs)
{
	return lhs._data == rhs._data;
}

template <typename T>
auto operator<<(std::ostream& os, Point<T> const& point) -> std::ostream&
{
	// https://eigen.tuxfamily.org/dox/structEigen_1_1IOFormat.html
	static Eigen::IOFormat const format(4, 0, ", ", "\n", "[", "]");

	auto vector = static_cast<typename project::Point<T>::container_type const&>(point);

	// Eigen defaults to column-major containers, so transpose to a row-major
	// vector for more-compact printing.
	os << vector.transpose().format(format);
	return os;
}

template <typename T>
template <typename Y>
auto Point<T>::cast() const -> Point<Y>
{
	return Point<Y>(_data.template cast<Y>());
}

}  // namespace project

#endif  // POINT_HXX
