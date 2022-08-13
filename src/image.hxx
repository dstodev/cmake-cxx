#ifndef IMAGE_HXX
#define IMAGE_HXX

#include <algorithm>
#include <exception>
#include <ostream>
#include <vector>

#include "image-utilities.hxx"
#include <project-export.h>

namespace project {

template <typename TPixel>
class Image;

template <typename TPixel>
std::ostream& operator<<(std::ostream& os, const Image<TPixel>& image);

/**
 * @brief An Image is a rendered arrangement of Pixels.
 *
 * This class arranges pixels in a rectangle:
 *    number-of-pixels = width (number of columns) * height (number of rows)
 *
 * @code
 * Pixels are accessed by row, column:
 *                  row  column
 *      | 0 1    a: 0    0
 *    --+----    b: 0    1
 *    0 | a b    c: 1    0
 *    1 | c d    d: 1    1
 * @endcode
 *
 * @tparam TPixel
 */
template <typename TPixel>
class Image
{
public:
	using value_type = TPixel;  // stl often names this specific member 'value_type'
	using container_type = std::vector<value_type>;

	using iterator = typename container_type::iterator;
	using const_iterator = typename container_type::const_iterator;

	explicit Image(dimension_type width = 0, dimension_type height = 0, container_type pixels = {});
	virtual ~Image() = default;

	Image(Image const& copy) = default;
	Image(Image&& move) noexcept = default;

	Image& operator=(Image const& copy) = default;
	Image& operator=(Image&& move) noexcept = default;

	[[nodiscard]] auto width() const -> dimension_type;
	void width(dimension_type width);

	[[nodiscard]] auto height() const -> dimension_type;
	void height(dimension_type height);

	auto pixels() const -> container_type const&;
	void pixels(container_type pixels);

	auto pixel(dimension_type row, dimension_type column) const -> value_type;
	void pixel(dimension_type row, dimension_type column, value_type value);

	// TODO: operator()(row,column) for pixel() shorthand

	auto begin() -> iterator;
	auto begin() const -> const_iterator;
	auto end() -> iterator;
	auto end() const -> const_iterator;

	// The '<>' after the operator name indicates that this friend is a template prototype
	friend std::ostream& operator<< <>(std::ostream& os, Image<value_type> const& image);

private:
	dimension_type _width;
	dimension_type _height;
	container_type _pixels;

	[[nodiscard]] auto pixel_index(dimension_type row, dimension_type column) const -> index_type;
};

template <typename TPixel>
Image<TPixel>::Image(dimension_type width, dimension_type height, container_type pixels)
    : _width(width)
    , _height(height)
    , _pixels(pixels)
{
	bool pixels_match_size = pixels.size() == _width * _height;
	bool pixels_provided = !pixels.empty();

	if (!pixels_match_size) {
		if (pixels_provided) {
			throw std::domain_error("Dimensions do not match number of provided pixels");
		}
		else {
			_pixels.resize(_width * _height);
		}
	}
}

template <typename TPixel>
auto Image<TPixel>::width() const -> dimension_type
{
	return _width;
}

template <typename TPixel>
void Image<TPixel>::width(dimension_type width)
{
	_width = width;
}

template <typename TPixel>
auto Image<TPixel>::height() const -> dimension_type
{
	return _height;
}

template <typename TPixel>
void Image<TPixel>::height(dimension_type height)
{
	_height = height;
}

template <typename TPixel>
auto Image<TPixel>::pixels() const -> container_type const&
{
	return _pixels;
}

template <typename TPixel>
void Image<TPixel>::pixels(container_type pixels)
{
	std::swap(_pixels, pixels);
}

template <typename TPixel>
auto Image<TPixel>::pixel(dimension_type row, dimension_type column) const -> value_type
{
	auto index = pixel_index(row, column);
	return _pixels[index];
}

template <typename TPixel>
void Image<TPixel>::pixel(dimension_type row, dimension_type column, value_type value)
{
	auto index = pixel_index(row, column);
	_pixels[index] = value;
}

template <typename TPixel>
auto Image<TPixel>::begin() -> iterator
{
	return _pixels.begin();
}

template <typename TPixel>
auto Image<TPixel>::begin() const -> const_iterator
{
	return _pixels.begin();
}

template <typename TPixel>
auto Image<TPixel>::end() -> iterator
{
	return _pixels.end();
}

template <typename TPixel>
auto Image<TPixel>::end() const -> const_iterator
{
	return _pixels.end();
}

template <typename TPixel>
std::ostream& operator<<(std::ostream& os, Image<TPixel> const& image)
{
	for (auto i = 0; i < image._width * image._height; ++i) {
		if (i % image._width == 0) {
			os << '\n';
		}
		os << image._pixels[i];
	}
	os << std::endl;
	return os;
}

template <typename TPixel>
auto Image<TPixel>::pixel_index(dimension_type row, dimension_type column) const -> index_type
{
	if (row >= _height) {
		throw std::domain_error("Row out of bounds");
	}
	if (column >= _width) {
		throw std::domain_error("Column out of bounds");
	}
	auto index = pixel_index_2d_to_1d(_width, row, column);
	return index;
}

}  // namespace project

#endif  // IMAGE_HXX
