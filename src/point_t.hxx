#ifndef POINT_T_HXX
#define POINT_T_HXX

#include <ostream>

#include <Eigen/Dense>

namespace project {

template <typename T>
class point_t;

template <typename T>
auto operator+=(point_t<T>& lhs, point_t<T> const& rhs) -> point_t<T>&;

template <typename T>
auto operator-=(point_t<T>& lhs, point_t<T> const& rhs) -> point_t<T>&;

template <typename T>
auto operator+(point_t<T> lhs, point_t<T> const& rhs) -> point_t<T>;

template <typename T>
auto operator-(point_t<T> lhs, point_t<T> const& rhs) -> point_t<T>;

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
	virtual ~point_t();

	point_t(point_t const& copy);
	explicit point_t(container_type const& copy);

	auto operator=(point_t const& copy) -> point_t&;
	auto operator=(container_type const& copy) -> point_t&;

	auto x() const -> value_type const&;
	auto x() -> value_type&;

	auto y() const -> value_type const&;
	auto y() -> value_type&;

	friend auto operator+=<>(point_t& lhs, point_t const& rhs) -> point_t&;
	friend auto operator-=<>(point_t& lhs, point_t const& rhs) -> point_t&;

	friend auto operator+<>(point_t lhs, point_t const& rhs) -> point_t;
	friend auto operator-<>(point_t lhs, point_t const& rhs) -> point_t;

	auto operator*=(value_type const& rhs) -> point_t&;
	auto operator/=(value_type const& rhs) -> point_t&;

	auto operator*(value_type const& rhs) -> point_t;
	auto operator/(value_type const& rhs) -> point_t;

	friend bool operator==<>(point_t const& lhs, point_t const& rhs);
	friend auto operator<< <>(std::ostream& os, point_t<value_type> const& point) -> std::ostream&;

	/* explicit */ operator container_type const&() const;
	/* explicit */ operator container_type&();

	template <typename Y>
	auto cast() const -> point_t<Y>;

private:
	container_type _data;
};

template <typename T>
point_t<T>::point_t(value_type value_x, value_type value_y)
    : _data {value_x, value_y}
{}

template <typename T>
point_t<T>::point_t(point_t const& copy)
    : _data {copy}
{}

template <typename T>
point_t<T>::point_t(container_type const& copy)
    : _data {copy}
{}

template <typename T>
point_t<T>::~point_t() = default;

template <typename T>
auto point_t<T>::operator=(point_t const& copy) -> point_t&
{
	_data = copy;
	return *this;
}

template <typename T>
auto point_t<T>::operator=(container_type const& copy) -> point_t&
{
	_data = copy;
	return *this;
}

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
auto operator+=(point_t<T>& lhs, point_t<T> const& rhs) -> point_t<T>&
{
	lhs._data += rhs._data;
	return lhs;
}

template <typename T>
auto operator-=(point_t<T>& lhs, point_t<T> const& rhs) -> point_t<T>&
{
	lhs._data -= rhs._data;
	return lhs;
}

template <typename T>
auto operator+(point_t<T> lhs, point_t<T> const& rhs) -> point_t<T>
{
	lhs += rhs;
	return lhs;
}

template <typename T>
auto operator-(point_t<T> lhs, point_t<T> const& rhs) -> point_t<T>
{
	lhs -= rhs;
	return lhs;
}

template <typename T>
auto point_t<T>::operator*=(value_type const& rhs) -> point_t<T>&
{
	_data *= rhs;
	return *this;
}

template <typename T>
auto point_t<T>::operator/=(value_type const& rhs) -> point_t<T>&
{
	_data /= rhs;
	return *this;
}

template <typename T>
auto point_t<T>::operator*(value_type const& rhs) -> point_t<T>
{
	auto result = *this;
	result *= rhs;
	return result;
}

template <typename T>
auto point_t<T>::operator/(value_type const& rhs) -> point_t<T>
{
	auto result = *this;
	result /= rhs;
	return result;
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
auto operator<<(std::ostream& os, point_t<T> const& point) -> std::ostream&
{
	// https://eigen.tuxfamily.org/dox/structEigen_1_1IOFormat.html
	static Eigen::IOFormat const format(4, 0, ", ", "\n", "[", "]");

	auto vector = static_cast<project::point_t<T>::container_type const&>(point);

	// Eigen defaults to column-major containers, so transpose to a row-major
	// vector for more-compact printing.
	os << vector.transpose().format(format);
	return os;
}

template <typename T>
template <typename Y>
auto point_t<T>::cast() const -> point_t<Y>
{
	return point_t<Y>(_data.template cast<Y>());
}

}  // namespace project

#endif  // POINT_T_HXX
