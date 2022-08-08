#ifndef IMAGE_HXX
#define IMAGE_HXX

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
 * This class arranges pixels in a rectangular pattern.
 * Pixels are accessed by row, column:
 *
 *              row column
 *     0 1   a: 0   0
 *   0 a b   b: 0   1
 *   1 c d   c: 1   0
 *           d: 1   1
 *
 * @tparam TPixel
 */
template <typename TPixel>
class Image
{
public:
	using container_t = std::vector<TPixel>;

	explicit Image(dimension_t width = 0, dimension_t height = 0, container_t pixels = {});
	virtual ~Image() = default;

	Image(Image const& copy) = default;
	Image(Image&& move) noexcept = default;

	Image& operator=(Image const& copy) = default;
	Image& operator=(Image&& move) noexcept = default;

	auto width() const { return _width; }
	void width(dimension_t width) { _width = width; }

	auto height() const { return _height; }
	void height(dimension_t height) { _height = height; }

	auto pixels() const -> container_t const& { return _pixels; }
	void pixels(container_t const& pixels) { _pixels = pixels; }

	auto pixel(dimension_t row, dimension_t column) const;

	// The '<>' after the operator name tells the compiler that this friend is a template
	friend std::ostream& operator<< <>(std::ostream& os, Image<TPixel> const& image);

private:
	dimension_t _width;
	dimension_t _height;
	container_t _pixels;
};

template <typename TPixel>
Image<TPixel>::Image(dimension_t width, dimension_t height, container_t pixels)
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
auto Image<TPixel>::pixel(dimension_t row, dimension_t column) const
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
