#ifndef GRID_HXX
#define GRID_HXX

#include <algorithm>
#include <exception>
#include <ostream>
#include <vector>

#include "utilities.hxx"
#include <project-api.h>

#if SUPPORT_EIGEN
#include <Eigen/Dense>
#endif

namespace project {

template <typename T>
class Grid;

template <typename T>
std::ostream& operator<<(std::ostream& os, const Grid<T>& grid);

/**
 * @brief A Grid is a two-dimensional arrangement of elements.
 *
 * This class arranges elements in a rectangle:
 *    number-of-elements = width (number of columns) * height (number of rows)
 *
 * @code
 * Elements are accessed by row, column:
 *      | 0 1       row  column
 *    --+----    a: 0    0
 *    0 | a b    b: 0    1
 *    1 | c d    c: 1    0
 *               d: 1    1
 * @endcode
 */
template <typename T>
class Grid
{
public:
	using value_type = T;  // stl often names this specific member 'value_type'
	using container_type = std::vector<value_type>;

	// Defer iterators to container
	using iterator = typename container_type::iterator;
	using const_iterator = typename container_type::const_iterator;

	virtual ~Grid() = default;
	explicit Grid(size_t width = 0, size_t height = 0, container_type data = {});

	Grid(Grid const& copy) = default;
	Grid(Grid&& move) noexcept = default;

	Grid& operator=(Grid const& copy) = default;
	Grid& operator=(Grid&& move) noexcept = default;

	[[nodiscard]] auto width() const -> size_t;
	void width(size_t width);

	[[nodiscard]] auto height() const -> size_t;
	void height(size_t height);

	auto data() const -> value_type const*;
	auto data() -> value_type*;
	void data(container_type data);  // Uses copy-swap idiom

	auto at(size_t row, size_t column) const -> value_type const&;
	auto at(size_t row, size_t column) -> value_type&;

	auto operator()(size_t row, size_t column) const -> value_type const&;  // Forwards to at()
	auto operator()(size_t row, size_t column) -> value_type&;  // Forwards to at()

	[[nodiscard]] auto size() const -> size_t;

	auto begin() -> iterator;
	auto begin() const -> const_iterator;
	auto end() -> iterator;
	auto end() const -> const_iterator;

	// The '<>' after the operator name indicates that this friend is a template prototype
	friend std::ostream& operator<< <>(std::ostream& os, Grid<value_type> const& grid);

#if SUPPORT_EIGEN
	template <int rows, int cols, int options = 0, int max_rows = rows, int max_cols = cols>
	explicit Grid(Eigen::Matrix<value_type, rows, cols, options, max_rows, max_cols> matrix);

	template <int rows, int cols, int options = 0, int max_rows = rows, int max_cols = cols>
	Eigen::Matrix<T, rows, cols, options | Eigen::RowMajor, max_rows, max_cols> as_matrix() const;
#endif

private:
	size_t _width;
	size_t _height;
	container_type _data;

	[[nodiscard]] auto element_index(size_t row, size_t column) const -> size_t;
};

template <typename T>
Grid<T>::Grid(size_t width, size_t height, container_type data)
    : _width(width)
    , _height(height)
    , _data(data)
{
	bool data_matches_size = size() == data.size();

	if (!data_matches_size) {
		bool data_is_default = data.empty();

		if (data_is_default) {
			_data.resize(size());
		}
		else {
			throw std::domain_error("Dimensions do not match size of provided data");
		}
	}
}

template <typename T>
auto Grid<T>::width() const -> size_t
{
	return _width;
}

template <typename T>
void Grid<T>::width(size_t width)
{
	// iterators in this function start from size() and iterate backwards
	// to preserve data-index continuity.

	if (width > _width) {
		// make me bigger
		_data.reserve(width * _height);
		auto const size_delta = width - _width;
		for (auto i {size()}; i > 0; i -= _width) {
			_data.insert(_data.begin() + i, size_delta, {});
		}
	}
	else if (width < _width) {
		// make me smaller
		auto const size_delta = _width - width;
		for (auto i {size()}; i > 0; i -= _width) {
			auto current_pos = _data.begin() + i;
			_data.erase(current_pos - size_delta, current_pos);
		}
	}

	_width = width;
}

template <typename T>
auto Grid<T>::height() const -> size_t
{
	return _height;
}

template <typename T>
void Grid<T>::height(size_t height)
{
	_height = height;
	_data.resize(size());
}

template <typename T>
auto Grid<T>::data() const -> value_type const*
{
	return _data.data();
}

template <typename T>
auto Grid<T>::data() -> value_type*
{
	return _data.data();
}

template <typename T>
void Grid<T>::data(container_type data)
{
	std::swap(_data, data);
}

template <typename T>
auto Grid<T>::at(size_t row, size_t column) const -> value_type const&
{
	auto index = element_index(row, column);
	return _data[index];
}

template <typename T>
auto Grid<T>::at(size_t row, size_t column) -> value_type&
{
	auto index = element_index(row, column);
	return _data[index];
}

template <typename T>
auto Grid<T>::operator()(size_t row, size_t column) const -> value_type const&
{
	return at(row, column);
}

template <typename T>
auto Grid<T>::operator()(size_t row, size_t column) -> value_type&
{
	return at(row, column);
}

template <typename T>
auto Grid<T>::size() const -> size_t
{
	return _width * _height;
}

template <typename T>
auto Grid<T>::begin() -> iterator
{
	return _data.begin();
}

template <typename T>
auto Grid<T>::begin() const -> const_iterator
{
	return _data.begin();
}

template <typename T>
auto Grid<T>::end() -> iterator
{
	return _data.end();
}

template <typename T>
auto Grid<T>::end() const -> const_iterator
{
	return _data.end();
}

template <typename T>
std::ostream& operator<<(std::ostream& os, Grid<T> const& grid)
{
	for (auto i {0}; i < grid._width * grid._height; ++i) {
		if (i % grid._width == 0) {
			os << '\n';
		}
		os << grid._data[i];
	}
	os << std::endl;
	return os;
}

template <typename T>
auto Grid<T>::element_index(size_t row, size_t column) const -> size_t
{
	if (row >= _height) {
		throw std::domain_error("Row out of bounds");
	}
	if (column >= _width) {
		throw std::domain_error("Column out of bounds");
	}
	auto index = index_2d_to_1d(_width, row, column);
	return index;
}

#if SUPPORT_EIGEN

template <typename T>
template <int rows, int cols, int options, int max_rows, int max_cols>
Grid<T>::Grid(Eigen::Matrix<value_type, rows, cols, options, max_rows, max_cols> matrix)
    : _data()
    , _width(matrix.cols())
    , _height(matrix.rows())
{
	if (matrix.IsRowMajor) {
		_data = std::vector<value_type>(matrix.data(), matrix.data() + matrix.size());
	}
	else {
		Eigen::Matrix<value_type, rows, cols, options | Eigen::RowMajor, max_rows, max_cols> row_matrix(matrix);
		_data = std::vector<value_type>(row_matrix.data(), row_matrix.data() + row_matrix.size());
	}
}

template <typename T>
template <int rows, int cols, int options, int max_rows, int max_cols>
Eigen::Matrix<T, rows, cols, options | Eigen::RowMajor, max_rows, max_cols> Grid<T>::as_matrix() const
{
	return Eigen::Matrix<value_type, rows, cols, options | Eigen::RowMajor, max_rows, max_cols>(data());
}

#endif  // SUPPORT_EIGEN

}  // namespace project

#endif  // GRID_HXX
