#include "image-utilities.hxx"

namespace project {

index_type pixel_index_2d_to_1d(dimension_type width, index_type row, index_type column)
{
	return row * width + column;
}

}  // namespace project
