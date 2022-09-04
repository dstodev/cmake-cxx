#include "utilities.hxx"

namespace project {

std::size_t index_2d_to_1d(std::size_t width, std::size_t row, std::size_t column)
{
	return row * width + column;
}

}  // namespace project
