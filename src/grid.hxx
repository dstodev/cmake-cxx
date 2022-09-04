#ifndef GRID_HXX
#define GRID_HXX

#include <algorithm>
#include <exception>
#include <ostream>
#include <vector>

#include "utilities.hxx"
#include <project-api.h>

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

	using iterator = typename container_type::iterator;
	using const_iterator = typename container_type::const_iterator;

	explicit Grid(std::size_t width = 0, std::size_t height = 0, container_type data = {});
	virtual ~Grid() = default;

	Grid(Grid const& copy) = default;
	Grid(Grid&& move) noexcept = default;

	Grid& operator=(Grid const& copy) = default;
	Grid& operator=(Grid&& move) noexcept = default;

	[[nodiscard]] auto width() const -> std::size_t;
	void width(std::size_t width);

	[[nodiscard]] auto height() const -> std::size_t;
	void height(std::size_t height);

	auto data() const -> container_type const&;
	auto data() -> container_type&;
	void data(container_type data);  // Uses copy-swap idiom

	auto at(std::size_t row, std::size_t column) const -> value_type const&;
	auto at(std::size_t row, std::size_t column) -> value_type&;

	// TODO: operator()(row,column) for at() shorthand

	auto begin() -> iterator;
	auto begin() const -> const_iterator;
	auto end() -> iterator;
	auto end() const -> const_iterator;

	// The '<>' after the operator name indicates that this friend is a template prototype
	friend std::ostream& operator<< <>(std::ostream& os, Grid<value_type> const& grid);

private:
	std::size_t _width;
	std::size_t _height;
	container_type _data;

	[[nodiscard]] auto element_index(std::size_t row, std::size_t column) const -> std::size_t;
};

template <typename T>
Grid<T>::Grid(std::size_t width, std::size_t height, container_type data)
    : _width(width)
    , _height(height)
    , _data(data)
{
	bool data_matches_size = data.size() == _width * _height;

	if (!data_matches_size) {
		bool data_is_default = data.empty();

		if (data_is_default) {
			_data.resize(_width * _height);
		}
		else {
			throw std::domain_error("Dimensions do not match size of provided data");
		}
	}
}

template <typename T>
auto Grid<T>::width() const -> std::size_t
{
	return _width;
}

template <typename T>
void Grid<T>::width(std::size_t width)
{
	// TODO: Resize data preserving shape
	_width = width;
}

template <typename T>
auto Grid<T>::height() const -> std::size_t
{
	return _height;
}

template <typename T>
void Grid<T>::height(std::size_t height)
{
	_height = height;
}

template <typename T>
auto Grid<T>::data() const -> container_type const&
{
	return _data;
}

template <typename T>
auto Grid<T>::data() -> Grid::container_type&
{
	return _data;
}

template <typename T>
void Grid<T>::data(container_type data)
{
	std::swap(_data, data);
}

template <typename T>
auto Grid<T>::at(std::size_t row, std::size_t column) const -> value_type const&
{
	auto index = element_index(row, column);
	return _data[index];
}

template <typename T>
auto Grid<T>::at(std::size_t row, std::size_t column) -> value_type&
{
	auto index = element_index(row, column);
	return _data[index];
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
auto Grid<T>::element_index(std::size_t row, std::size_t column) const -> std::size_t
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

}  // namespace project

#endif  // GRID_HXX
