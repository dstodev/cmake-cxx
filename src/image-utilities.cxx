#include "image-utilities.hxx"

namespace project {

index_t pixel_index_2d_to_1d(dimension_t stride, index_t row, index_t column)
{
	return stride * row + column;
}

}  // namespace project
