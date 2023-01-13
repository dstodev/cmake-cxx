#ifndef POINT_T_HXX
#define POINT_T_HXX

#include <ostream>

#include <Eigen/Dense>

#include <project-api.h>

namespace project {

class PROJECT_API point_t
{
public:
	using container_type = Eigen::Vector2f;
	using value_type = container_type::value_type;

	explicit point_t(value_type x_val = {}, value_type y_val = {})
	    : _vec {x_val, y_val}
	    , x {_vec[0]}
	    , y {_vec[1]}
	{}

	virtual ~point_t() = default;

	point_t& operator=(container_type const& copy);

	PROJECT_API friend bool operator==(point_t const& lhs, point_t const& rhs);

	PROJECT_API friend std::ostream& operator<<(std::ostream& os, point_t const& point);

	operator container_type&();
	operator container_type const&() const;

private:
	container_type _vec;

public:
	value_type& x;
	value_type& y;
};

PROJECT_API bool operator==(point_t const& lhs, point_t const& rhs);
PROJECT_API std::ostream& operator<<(std::ostream& os, point_t const& point);

}  // namespace project

#endif  // POINT_T_HXX
