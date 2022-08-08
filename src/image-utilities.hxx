#ifndef IMAGE_UTILITIES_HXX
#define IMAGE_UTILITIES_HXX

#include <cstddef>

#include <project_export.h>

namespace project {

using dimension_t = std::size_t;
using index_t = std::size_t;

PROJECT_API index_t pixel_index_2d_to_1d(dimension_t stride, index_t row, index_t column);

}  // namespace project

#endif  // IMAGE_UTILITIES_HXX
