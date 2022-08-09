#ifndef IMAGE_UTILITIES_HXX
#define IMAGE_UTILITIES_HXX

#include <cstddef>

#include <project_export.h>

namespace project {

using dimension_type = std::size_t;
using index_type = std::size_t;

/** @brief Translate a row,column coordinate to 1-dimensional ordinal index
 *
 *    image       inputs, width=2  | output
 *                  | row | column | index
 *      | 0 1    a: | 0   | 0      | 0
 *    --+----    b: | 0   | 1      | 1
 *    0 | a b    c: | 1   | 0      | 2
 *    1 | c d    d: | 1   | 1      | 3
 *
 * @param width Width of the image; number of columns
 * @param row Row of target index
 * @param column Column of target index
 * @return 1D index of coordinate row,column
 */
PROJECT_API index_type pixel_index_2d_to_1d(dimension_type width, index_type row, index_type column);

}  // namespace project

#endif  // IMAGE_UTILITIES_HXX
