#include "utilities.hxx"

namespace project {

size_t index_2d_to_1d(size_t width, size_t row, size_t column)
{
	return row * width + column;
}

}  // namespace project
