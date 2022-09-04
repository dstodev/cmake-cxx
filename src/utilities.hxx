#ifndef UTILITIES_HXX
#define UTILITIES_HXX

#include <cstddef>

#include <project-api.h>

namespace project {

/** @brief Translate a row,column coordinate to 1-dimensional ordinal index
 *
 * @code
 *               inputs, width=2  | output
 *      | 0 1       | row | column | index
 *    --+----    a: | 0   | 0      | 0
 *    0 | a b    b: | 0   | 1      | 1
 *    1 | c d    c: | 1   | 0      | 2
 *               d: | 1   | 1      | 3
 * @endcode
 *
 * @param stride Total number of columns
 * @param row Row of target index
 * @param column Column of target index
 * @return 1D index of coordinate row,column
 */
PROJECT_API std::size_t index_2d_to_1d(std::size_t stride, std::size_t row, std::size_t column);

}  // namespace project

#endif  // UTILITIES_HXX
