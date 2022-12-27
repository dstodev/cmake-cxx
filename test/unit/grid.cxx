#include <gtest/gtest.h>

#include <string>

#include "assertions.hxx"
#include <grid.hxx>

using namespace project;

#if SUPPORT_EIGEN
using namespace Eigen;
#endif

TEST(Grid, construct)
{
	Grid<char> o;
	ASSERT_EQ(0, o.width());
	ASSERT_EQ(0, o.height());
}

TEST(Grid, construct_non_default)
{
	Grid<char> o(0, 1);
	ASSERT_EQ(0, o.width());
	ASSERT_EQ(1, o.height());
}

TEST(Grid, construct_copy)
{
	Grid<char> o(0, 1);
	Grid<char> copy(o);
	ASSERT_EQ(0, copy.width());
	ASSERT_EQ(1, copy.height());
}

TEST(Grid, setters)
{
	Grid<char> o(0, 0);
	o.width(1);
	o.height(2);
	ASSERT_EQ(1, o.width());
	ASSERT_EQ(2, o.height());

	o.data(decltype(o)::container_type {3, 4});

	auto const& data = o.data();
	ASSERT_EQ(3, data[0]);
	ASSERT_EQ(4, data[1]);
}

TEST(Grid, size)
{
	Grid<char> o(2, 3);
	ASSERT_EQ(6, o.size());
}

TEST(Grid, construct_with_invalid_data_dimensions)
{
	ASSERT_ANY_THROW(Grid<char> o(0, 0, {1}));
}

TEST(Grid, construct_with_data)
{
	Grid<char> o(1, 2, {3, 4});
	auto* data = o.data();
	ASSERT_EQ(3, data[0]);
	ASSERT_EQ(4, data[1]);
}

TEST(Grid, data_const)
{
	Grid<char> const o(1, 2, {3, 4});
	auto const* data = o.data();
	ASSERT_EQ(3, data[0]);
	ASSERT_EQ(4, data[1]);
}

TEST(Grid, at_horizontal_rectangle)
{
	/*  0 1 2 3 4   a b c d e f g h i j
	  0 a b c d e   0 1 2 3 4 5 6 7 8 9
	  1 f g h i j  */
	Grid<char> o(5, 2, {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j'});
	ASSERT_EQ('h', o.at(1, 2)) << o;
}

TEST(Grid, at_vertical_rectangle)
{
	/*  0 1   a b c d e f g h i j
	  0 a b   0 1 2 3 4 5 6 7 8 9
	  1 c d
	  2 e f
	  3 g h
	  4 i j  */
	Grid<char> o(2, 5, {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j'});
	ASSERT_EQ('f', o.at(2, 1)) << o;
}

TEST(Grid, at_row_out_of_bounds)
{
	Grid<char> o(1, 1);
	o.at(0, 0);
	ASSERT_ANY_THROW(o.at(1, 0)) << o;
}

TEST(Grid, at_column_out_of_bounds)
{
	Grid<char> o(1, 1);
	o.at(0, 0);
	ASSERT_ANY_THROW(o.at(0, 1)) << o;
}

TEST(Grid, at_set)
{
	Grid<char> o(1, 2, {3, 4});
	o.at(1, 0) = 5;
	ASSERT_EQ(5, o.at(1, 0)) << o;
}

TEST(Grid, at_set_row_out_of_bounds)
{
	Grid<char> o(1, 1);
	o.at(0, 0) = 1;
	ASSERT_ANY_THROW(o.at(1, 0) = 2) << o;
}

TEST(Grid, at_set_column_out_of_bounds)
{
	Grid<char> o(1, 1);
	o.at(0, 0) = 1;
	ASSERT_ANY_THROW(o.at(0, 1) = 2) << o;
}

TEST(Grid, at_const)
{
	Grid<char> const o(1, 1);
	auto const& element = o.at(0, 0);
	ASSERT_EQ(0, element);
}

TEST(Grid, call_operator)
{
	Grid<char> o(1, 1);

	ASSERT_EQ(0, o(0, 0));
	o(0, 0) = 1;
	ASSERT_EQ(1, o(0, 0));

	ASSERT_ANY_THROW(o(1, 0) = 2) << o;
	ASSERT_ANY_THROW(o(0, 1) = 2) << o;
}

TEST(Grid, call_operator_const)
{
	Grid<char> const o(1, 1);
	auto const& element = o(0, 0);
	ASSERT_EQ(0, element);
}

TEST(Grid, iterator)
{
	Grid<char> o(2, 2, {0, 1, 2, 3});

	// Assert that the non-const iterator is used with a non-const grid
	using grid_type = decltype(o);
	MY_ASSERT_SAME_TYPE(grid_type::iterator, decltype(std::declval<grid_type>().begin()));
	MY_ASSERT_SAME_TYPE(grid_type::iterator, decltype(std::declval<grid_type>().end()));

	for (auto i : o) {
		ASSERT_EQ(i, o.data()[i]);
	}
}

TEST(Grid, iterator_mutates)
{
	Grid<char> o(2, 2, {0, 1, 2, 3});
	for (auto& i : o) {
		auto index = i;
		i += 1;
		ASSERT_EQ(index + 1, o.data()[index]);
	}
}

TEST(Grid, const_iterator)
{
	Grid<char> const o(2, 2, {0, 1, 2, 3});

	// Assert that the const iterator is used with a const grid
	using grid_type = decltype(o);
	MY_ASSERT_SAME_TYPE(grid_type::const_iterator, decltype(std::declval<grid_type>().begin()));
	MY_ASSERT_SAME_TYPE(grid_type::const_iterator, decltype(std::declval<grid_type>().end()));

	for (auto i : o) {
		ASSERT_EQ(i, o.data()[i]);
	}
}

TEST(Grid, change_height_maintains_data_consistency)
{
	Grid<char> o(2, 2, {0, 1, 2, 3});
	ASSERT_EQ(4, o.size());

	/*  0 1
	  0 0 1
	  1 2 3  */
	ASSERT_EQ(0, o.at(0, 0));
	ASSERT_EQ(1, o.at(0, 1));
	ASSERT_EQ(2, o.at(1, 0));
	ASSERT_EQ(3, o.at(1, 1));

	o.height(4);
	ASSERT_EQ(8, o.size());

	/*  0 1
	  0 0 1
	  1 2 3
	  2 0 0
	  3 0 0  */
	ASSERT_EQ(0, o.at(0, 0));
	ASSERT_EQ(1, o.at(0, 1));
	ASSERT_EQ(2, o.at(1, 0));
	ASSERT_EQ(3, o.at(1, 1));
	ASSERT_EQ(0, o.at(2, 0));
	ASSERT_EQ(0, o.at(2, 1));
	ASSERT_EQ(0, o.at(3, 0));
	ASSERT_EQ(0, o.at(3, 1));

	o.height(2);
	ASSERT_EQ(4, o.size());

	/*  0 1
	  0 0 1
	  1 2 3  */
	ASSERT_EQ(0, o.at(0, 0));
	ASSERT_EQ(1, o.at(0, 1));
	ASSERT_EQ(2, o.at(1, 0));
	ASSERT_EQ(3, o.at(1, 1));
}

TEST(Grid, change_width_maintains_data_consistency)
{
	Grid<char> o(2, 2, {0, 1, 2, 3});
	ASSERT_EQ(4, o.size());

	/*  0 1
	  0 0 1
	  1 2 3  */
	ASSERT_EQ(0, o.at(0, 0));
	ASSERT_EQ(1, o.at(0, 1));
	ASSERT_EQ(2, o.at(1, 0));
	ASSERT_EQ(3, o.at(1, 1));

	o.width(4);
	ASSERT_EQ(8, o.size());

	/*  0 1 2 3
	  0 0 1 0 0
	  1 2 3 0 0  */
	ASSERT_EQ(0, o.at(0, 0));
	ASSERT_EQ(1, o.at(0, 1));
	ASSERT_EQ(0, o.at(0, 2));
	ASSERT_EQ(0, o.at(0, 3));
	ASSERT_EQ(2, o.at(1, 0));
	ASSERT_EQ(3, o.at(1, 1));
	ASSERT_EQ(0, o.at(1, 2));
	ASSERT_EQ(0, o.at(1, 3));

	o.width(2);
	ASSERT_EQ(4, o.size());

	/*  0 1
	  0 0 1
	  1 2 3  */
	ASSERT_EQ(0, o.at(0, 0));
	ASSERT_EQ(1, o.at(0, 1));
	ASSERT_EQ(2, o.at(1, 0));
	ASSERT_EQ(3, o.at(1, 1));
}

#if SUPPORT_EIGEN
TEST(Grid, eigen_interaction)
{
	Grid<char> grid(2, 2, {0, 1, 2, 3});
	/*     x=0  x=1  x=column
	  y=0  0    1    y=row
	  y=1  2    3          */

	// Grid is indexed like (row, column) = (y, x)
	ASSERT_EQ(0, grid(0, 0));
	ASSERT_EQ(1, grid(0, 1));
	ASSERT_EQ(2, grid(1, 0));
	ASSERT_EQ(3, grid(1, 1));

	// Matrix is indexed like (x, y) = (column, row)
	auto matrix = grid.as_matrix<2, 2>();
	ASSERT_EQ(0, matrix(0, 0));
	ASSERT_EQ(1, matrix(1, 0));
	ASSERT_EQ(2, matrix(0, 1));
	ASSERT_EQ(3, matrix(1, 1));

	matrix(0, 0) = 4;
	ASSERT_EQ(4, matrix(0, 0));
	ASSERT_EQ(0, grid(0, 0));  // grid is unchanged
}
#endif  // SUPPORT_EIGEN
