#ifndef IMAGE_HXX
#define IMAGE_HXX

#include <algorithm>
#include <exception>
#include <ostream>
#include <vector>

#include "image-utilities.hxx"
#include <project_export.h>

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
 * Pixels are accessed by row, column:
 *                   row column
 *      | 0 1    a: 0   0
 *    --+----    b: 0   1
 *    0 | a b    c: 1   0
 *    1 | c d    d: 1   1
 *
 * @tparam TPixel
 */
template <typename TPixel>
class Image
{
public:
	using value_type = TPixel;  // stl often names this specific member 'value_type'
	using container_type = std::vector<value_type>;

	explicit Image(dimension_type width = 0, dimension_type height = 0, container_type pixels = {});
	virtual ~Image() = default;

	Image(Image const& copy) = default;
	Image(Image&& move) noexcept = default;

	Image& operator=(Image const& copy) = default;
	Image& operator=(Image&& move) noexcept = default;

	auto width() const;
	void width(dimension_type width);

	auto height() const;
	void height(dimension_type height);

	auto pixels() const -> container_type const&;
	void pixels(container_type pixels);

	auto pixel(dimension_type row, dimension_type column) const;

	// The '<>' after the operator name indicates that this friend is a template prototype
	friend std::ostream& operator<< <>(std::ostream& os, Image<value_type> const& image);

private:
	dimension_type _width;
	dimension_type _height;
	container_type _pixels;
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
			throw std::domain_error("Dimensions do not match length of provided pixels");
		}
		else {
			_pixels.resize(_width * _height);
		}
	}
}

template <typename TPixel>
auto Image<TPixel>::width() const
{
	return _width;
}

template <typename TPixel>
void Image<TPixel>::width(dimension_type width)
{
	_width = width;
}

template <typename TPixel>
auto Image<TPixel>::height() const
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
auto Image<TPixel>::pixel(dimension_type row, dimension_type column) const
{
	if (row >= _height) {
		throw std::domain_error("Row out of bounds");
	}
	if (column >= _width) {
		throw std::domain_error("Column out of bounds");
	}
	auto index = pixel_index_2d_to_1d(_width, row, column);
	return _pixels[index];
}

template <typename TPixel>
std::ostream& operator<<(std::ostream& os, const Image<TPixel>& image)
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

}  // namespace project

#endif  // IMAGE_HXX
