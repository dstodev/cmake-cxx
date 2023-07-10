#ifndef GRID_T_HXX
#define GRID_T_HXX

#include <iterator>
#include <ostream>
#include <type_traits>

#include <project-api.h>

#include <Eigen/Dense>

namespace project {

namespace detail {
// https://eigen.tuxfamily.org/dox/classEigen_1_1Matrix.html
template <typename T, int rows, int cols, int options = 0, int max_rows = rows, int max_cols = cols>
using RowMajor = Eigen::Matrix<T, rows, cols, options | Eigen::RowMajor, max_rows, max_cols>;

template <typename T, int rows, int cols, int options = 0, int max_rows = rows, int max_cols = cols>
using ColMajor = Eigen::Matrix<T, rows, cols, options | Eigen::ColMajor, max_rows, max_cols>;

template <typename Tvalue>
using iterator_base = std::iterator<std::forward_iterator_tag, Tvalue>;
}  // namespace detail

template <typename T>
class grid_t;

template <typename T>
std::ostream& operator<<(std::ostream& os, grid_t<T> const& grid);

/** @brief A Grid is a two-dimensional arrangement of elements.

    This class arranges elements in a rectangle:
       number-of-elements = width (number of columns) * height (number of rows)

@code
    Elements are accessed by row, column:

         | 0 1       row  column
       --+----    a: 0    0
       0 | a b    b: 0    1
       1 | c d    c: 1    0
                  d: 1    1
@endcode
 */
template <typename T>
class grid_t
{
public:
	using value_type = T;  // stl often names this specific member 'value_type'
	using container_type = detail::ColMajor<value_type, Eigen::Dynamic, Eigen::Dynamic>;

	template <typename Tvalue, typename Tcontainer, bool const_access = true>
	struct iterator_impl;

	using const_iterator = iterator_impl<value_type const, container_type const>;
	using iterator = iterator_impl<value_type, container_type, false>;

	virtual ~grid_t() = default;
	grid_t() = default;

	/** @brief Grid constructor
	    @param height Number of grid rows
	    @param width Number of grid columns
	    @param init One-dimensional list of data to initialize with, interpreted
	                in row-major order. Must have at least width * height elements.
	 */
	explicit grid_t(size_t height, size_t width, std::initializer_list<value_type> init = {});

	template <typename M>
	explicit grid_t(M const& matrix);

	grid_t(grid_t const& copy) = default;
	grid_t(grid_t&& move) noexcept = default;

	grid_t& operator=(grid_t const& copy) = default;
	grid_t& operator=(grid_t&& move) noexcept = default;

	/// Number of grid rows
	[[nodiscard]]
	auto height() const -> size_t;
	void height(size_t height);

	/// Number of grid columns
	[[nodiscard]]
	auto width() const -> size_t;
	void width(size_t width);

	auto at(size_t row, size_t column) const -> value_type const&;
	auto at(size_t row, size_t column) -> value_type&;

	// operator() forwards to at()
	auto operator()(size_t row, size_t column) const -> value_type const&;
	auto operator()(size_t row, size_t column) -> value_type&;

	[[nodiscard]]
	auto size() const -> size_t;

	auto as_matrix() const -> container_type const&;
	auto as_matrix() -> container_type&;

	explicit operator container_type const&() const;
	explicit operator container_type&();

	auto begin() const -> const_iterator;
	auto begin() -> iterator;
	auto end() const -> const_iterator;
	auto end() -> iterator;

	// The '<>' after the operator name indicates that this friend is a template prototype
	friend std::ostream& operator<< <>(std::ostream& os, grid_t<value_type> const& grid);

private:
	container_type _data;
};

template <typename T>
grid_t<T>::grid_t(size_t height, size_t width, std::initializer_list<value_type> init)
    : _data(height, width)
{
	auto const default_value = value_type {};
	_data.fill(default_value);

	if (!std::empty(init)) {
		if (_data.size() != init.size()) {
			throw std::domain_error("Dimensions do not match number of provided data");
		}
		for (auto row {0}; row < height; ++row) {
			for (auto col {0}; col < width; ++col) {
				_data(row, col) = init.begin()[row * width + col];
			}
		}
	}
}

template <typename T>
template <typename M>
grid_t<T>::grid_t(M const& matrix)
    : _data(matrix)
{}

template <typename T>
auto grid_t<T>::height() const -> size_t
{
	return _data.rows();
}

template <typename T>
void grid_t<T>::height(size_t height)
{
	auto const previous_height = this->height();
	_data.conservativeResize(height, Eigen::NoChange);

	if (height > previous_height) {
		auto const default_value = value_type {};
		auto const num_added_rows = height - previous_height;
		_data.bottomRows(num_added_rows).setConstant(default_value);
	}
}

template <typename T>
auto grid_t<T>::width() const -> size_t
{
	return _data.cols();
}

template <typename T>
void grid_t<T>::width(size_t width)
{
	auto const previous_width = this->width();
	_data.conservativeResize(Eigen::NoChange, width);

	if (width > previous_width) {
		auto const default_value = value_type {};
		auto const num_added_columns = width - previous_width;
		_data.rightCols(num_added_columns).setConstant(default_value);
	}
}

template <typename T>
auto grid_t<T>::at(size_t row, size_t column) const -> value_type const&
{
	if (row >= _data.rows()) {
		throw std::domain_error("Row out of bounds");
	}
	if (column >= _data.cols()) {
		throw std::domain_error("Column out of bounds");
	}
	return _data(row, column);
}

template <typename T>
auto grid_t<T>::at(size_t row, size_t column) -> value_type&
{
	/* To reduce code duplication between `at()` and `at() const`,
	   add const to `this` pointer to unambiguously call `at() const`,
	   call it, then const_cast<> away the const.
	   This is safe because this function is not const (`at()` not `at() const`):
	   the only way to call `at()` is via non-const instance or reference,
	   guaranteeing that calling const_cast<> to remove const here is safe.
	 */
	auto const* const_this = this;
	return const_cast<value_type&>(const_this->at(row, column));
}

template <typename T>
auto grid_t<T>::operator()(size_t row, size_t column) const -> value_type const&
{
	return at(row, column);
}

template <typename T>
auto grid_t<T>::operator()(size_t row, size_t column) -> value_type&
{
	return at(row, column);
}

template <typename T>
auto grid_t<T>::size() const -> size_t
{
	return _data.size();
}

template <typename T>
auto grid_t<T>::as_matrix() const -> container_type const&
{
	return _data;
}

template <typename T>
auto grid_t<T>::as_matrix() -> container_type&
{
	return _data;
}

template <typename T>
grid_t<T>::operator container_type const&() const
{
	return _data;
}

template <typename T>
grid_t<T>::operator container_type&()
{
	return _data;
}

template <typename T>
auto grid_t<T>::begin() const -> const_iterator
{
	return const_iterator(_data);
}

template <typename T>
auto grid_t<T>::begin() -> iterator
{
	return iterator(_data);
}

template <typename T>
auto grid_t<T>::end() const -> const_iterator
{
	return const_iterator(_data, _data.size());
}

template <typename T>
auto grid_t<T>::end() -> iterator
{
	return iterator(_data, _data.size());
}

template <typename T>
std::ostream& operator<<(std::ostream& os, grid_t<T> const& grid)
{
	for (auto i {0}; i < grid.width() * grid.height(); ++i) {
		if (i % grid.width() == 0) {
			os << '\n';
		}
		os << grid.begin()[i];
	}
	os << std::endl;
	return os;
}

template <typename T>
template <typename Tvalue, typename Tcontainer, bool const_access>
struct grid_t<T>::iterator_impl : detail::iterator_base<Tvalue>
{
	using container_type = Tcontainer;
	using typename detail::iterator_base<Tvalue>::difference_type;
	using typename detail::iterator_base<Tvalue>::reference;

	container_type& container;
	difference_type position;

	explicit iterator_impl(container_type& container, difference_type position = {})
	    : container {container}
	    , position {position}
	{}

	auto operator++() -> iterator_impl&  // ++prefix
	{
		++position;
		return *this;
	}

	auto operator++(int) -> iterator_impl  // postfix++
	{
		auto const value = *this;
		++(*this);
		return value;
	}

	bool operator==(iterator_impl const& other) const
	{
		return (&container == &other.container) && (position == other.position);
	}

	bool operator!=(iterator_impl const& other) const
	{
		bool const equal = *this == other;
		return !equal;
	}

	template <bool _const_access = const_access>
	auto operator*() const -> std::enable_if_t<_const_access, reference>
	{
		return (*this)[position];
	}

	template <bool _const_access = const_access>
	auto operator*() -> std::enable_if_t<!_const_access, reference>
	{
		return (*this)[position];
	}

	auto operator[](difference_type index) const -> value_type const&
	{
		// https://eigen.tuxfamily.org/dox/group__TutorialSTL.html
		auto columns = container.cols();
		auto row = index / columns;
		auto column = index % columns;
		return container(row, column);
	}

	template <bool _const_access = const_access>
	auto operator[](difference_type index) -> std::enable_if_t<!_const_access, reference>
	{
		auto const* const_this = this;
		return const_cast<value_type&>((*const_this)[index]);
	}
};

}  // namespace project

#endif  // GRID_T_HXX
