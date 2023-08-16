#include <gtest/gtest.h>

#include "assertions.hxx"
#include <grid-t.hxx>

using namespace project;

using namespace Eigen;

TEST(Grid, construct)
{
	grid_t<char> grid;
	ASSERT_EQ(0, grid.width());
	ASSERT_EQ(0, grid.height());
}

TEST(Grid, construct_non_default)
{
	grid_t<char> grid(1, 0);
	ASSERT_EQ(0, grid.width());
	ASSERT_EQ(1, grid.height());
}

TEST(Grid, construct_copy)
{
	grid_t<char> grid(1, 0);
	grid_t<char> copy(grid);
	ASSERT_EQ(0, copy.width());
	ASSERT_EQ(1, copy.height());
}

TEST(Grid, setters)
{
	grid_t<char> grid(0, 0);
	grid.width(1);
	grid.height(2);
	ASSERT_EQ(1, grid.width());
	ASSERT_EQ(2, grid.height());
}

TEST(Grid, size)
{
	grid_t<char> grid(3, 2);
	ASSERT_EQ(6, grid.size());
}

TEST(Grid, construct_with_invalid_data_dimensions)
{
	ASSERT_ANY_THROW(grid_t<char> grid(0, 0, {1}));
}

TEST(Grid, construct_with_data)
{
	grid_t<char> grid(2, 1, {3, 4});
	ASSERT_EQ(3, grid.at(0, 0));
	ASSERT_EQ(4, grid.at(1, 0));
}

TEST(Grid, construct_with_nonscalar_data)
{
	struct Object
	{
		int value;
	};
	grid_t<Object> grid(2, 1, {Object {3}, Object {4}});
	ASSERT_EQ(3, grid.at(0, 0).value);
	ASSERT_EQ(4, grid.at(1, 0).value);
}

/* TODO: The previous test only passes because Object is constructible like Object{}.
       Investigate supporting non-default-constructible types: */

#if brave
TEST(Grid, construct_with_non_default_constructible_data)
{
	struct Object
	{
		explicit Object(int value)
		    : value(value)
		{}
		int value;
	};
	grid_t<Object> grid(2, 1, {Object(3), Object(4)});
	ASSERT_EQ(3, grid.at(0, 0).value);
	ASSERT_EQ(4, grid.at(1, 0).value);
}
#endif

TEST(Grid, at_horizontal_rectangle)
{
	/*  0 1 2 3 4   a b c d e f g h i j
	  0 a b c d e   0 1 2 3 4 5 6 7 8 9
	  1 f g h i j  */
	grid_t<char> grid(2, 5, {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j'});
	ASSERT_EQ('h', grid.at(1, 2)) << grid;
}

TEST(Grid, at_vertical_rectangle)
{
	/*  0 1   a b c d e f g h i j
	  0 a b   0 1 2 3 4 5 6 7 8 9
	  1 c d
	  2 e f
	  3 g h
	  4 i j  */
	grid_t<char> grid(5, 2, {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j'});
	ASSERT_EQ('f', grid.at(2, 1)) << grid;
}

TEST(Grid, at_row_out_of_bounds)
{
	grid_t<char> grid(1, 1);
	grid.at(0, 0);
	ASSERT_ANY_THROW(grid.at(1, 0)) << grid;
}

TEST(Grid, at_column_out_of_bounds)
{
	grid_t<char> grid(1, 1);
	grid.at(0, 0);
	ASSERT_ANY_THROW(grid.at(0, 1)) << grid;
}

TEST(Grid, at_set)
{
	grid_t<char> grid(2, 1);
	grid.at(1, 0) = 5;
	ASSERT_EQ(5, grid.at(1, 0)) << grid;
}

TEST(Grid, at_set_row_out_of_bounds)
{
	grid_t<char> grid(1, 1);
	grid.at(0, 0) = 1;
	ASSERT_ANY_THROW(grid.at(1, 0) = 2) << grid;
}

TEST(Grid, at_set_column_out_of_bounds)
{
	grid_t<char> grid(1, 1);
	grid.at(0, 0) = 1;
	ASSERT_ANY_THROW(grid.at(0, 1) = 2) << grid;
}

TEST(Grid, at_const)
{
	grid_t<char> const grid(1, 1);
	auto const& element = grid.at(0, 0);
	ASSERT_EQ(0, element);
}

TEST(Grid, call_operator)
{
	grid_t<char> grid(1, 1);

	ASSERT_EQ(0, grid(0, 0));
	grid(0, 0) = 1;
	ASSERT_EQ(1, grid(0, 0));

	ASSERT_ANY_THROW(grid(1, 0) = 2) << grid;
	ASSERT_ANY_THROW(grid(0, 1) = 2) << grid;
}

TEST(Grid, call_operator_const)
{
	grid_t<char> const grid(1, 1);
	auto const& element = grid(0, 0);
	ASSERT_EQ(0, element);
}

TEST(Grid, iterator)  // griderator
{
	grid_t<char> grid(2, 2, {0, 1, 2, 3});

	// Assert that the non-const iterator is used with a non-const grid
	using grid_type = decltype(grid);
	MY_ASSERT_SAME_TYPE(grid_type::iterator, decltype(std::declval<grid_type>().begin()));
	MY_ASSERT_SAME_TYPE(grid_type::iterator, decltype(std::declval<grid_type>().end()));

	auto i {0};

	for (auto it : grid) {
		EXPECT_EQ(i++, it);
	}
	ASSERT_EQ(grid.size(), i);
}

TEST(Grid, iterator_mutates)
{
	grid_t<char> grid(2, 2, {0, 1, 2, 3});

	for (auto& i : grid) {
		auto index = i;  // copy; value is also value's index
		i += 1;
		EXPECT_EQ(index + 1, grid.begin()[index]);
	}
}

TEST(Grid, const_iterator)
{
	grid_t<char> const grid(2, 2, {0, 1, 2, 3});

	// Assert that the const iterator is used with a const grid
	using grid_type = decltype(grid);
	MY_ASSERT_SAME_TYPE(grid_type::const_iterator, decltype(std::declval<grid_type>().begin()));
	MY_ASSERT_SAME_TYPE(grid_type::const_iterator, decltype(std::declval<grid_type>().end()));

	for (auto i : grid) {
		EXPECT_EQ(i, grid.begin()[i]);
	}
}

TEST(Grid, change_height_maintains_data_consistency)
{
	grid_t<char> grid(2, 2, {0, 1, 2, 3});
	ASSERT_EQ(4, grid.size());

	/*  0 1
	  0 0 1
	  1 2 3  */
	ASSERT_EQ(0, grid.at(0, 0));
	ASSERT_EQ(1, grid.at(0, 1));
	ASSERT_EQ(2, grid.at(1, 0));
	ASSERT_EQ(3, grid.at(1, 1));

	grid.height(4);
	ASSERT_EQ(8, grid.size());

	/*  0 1
	  0 0 1
	  1 2 3
	  2 0 0
	  3 0 0  */
	ASSERT_EQ(0, grid.at(0, 0));
	ASSERT_EQ(1, grid.at(0, 1));
	ASSERT_EQ(2, grid.at(1, 0));
	ASSERT_EQ(3, grid.at(1, 1));
	ASSERT_EQ(0, grid.at(2, 0));
	ASSERT_EQ(0, grid.at(2, 1));
	ASSERT_EQ(0, grid.at(3, 0));
	ASSERT_EQ(0, grid.at(3, 1));

	grid.height(2);
	ASSERT_EQ(4, grid.size());

	/*  0 1
	  0 0 1
	  1 2 3  */
	ASSERT_EQ(0, grid.at(0, 0));
	ASSERT_EQ(1, grid.at(0, 1));
	ASSERT_EQ(2, grid.at(1, 0));
	ASSERT_EQ(3, grid.at(1, 1));
}

TEST(Grid, change_width_maintains_data_consistency)
{
	grid_t<char> grid(2, 2, {0, 1, 2, 3});
	ASSERT_EQ(4, grid.size());

	/*  0 1
	  0 0 1
	  1 2 3  */
	ASSERT_EQ(0, grid.at(0, 0));
	ASSERT_EQ(1, grid.at(0, 1));
	ASSERT_EQ(2, grid.at(1, 0));
	ASSERT_EQ(3, grid.at(1, 1));

	grid.width(4);
	ASSERT_EQ(8, grid.size());

	/*  0 1 2 3
	  0 0 1 0 0
	  1 2 3 0 0  */
	ASSERT_EQ(0, grid.at(0, 0));
	ASSERT_EQ(1, grid.at(0, 1));
	ASSERT_EQ(0, grid.at(0, 2));
	ASSERT_EQ(0, grid.at(0, 3));
	ASSERT_EQ(2, grid.at(1, 0));
	ASSERT_EQ(3, grid.at(1, 1));
	ASSERT_EQ(0, grid.at(1, 2));
	ASSERT_EQ(0, grid.at(1, 3));

	grid.width(2);
	ASSERT_EQ(4, grid.size());

	/*  0 1
	  0 0 1
	  1 2 3  */
	ASSERT_EQ(0, grid.at(0, 0));
	ASSERT_EQ(1, grid.at(0, 1));
	ASSERT_EQ(2, grid.at(1, 0));
	ASSERT_EQ(3, grid.at(1, 1));
}

// https://github.com/google/googletest/blob/main/docs/primer.md#test-fixtures-using-the-same-data-configuration-for-multiple-tests-same-data-multiple-tests
class GridToFromEigenMatrix : public ::testing::Test
{
protected:
	grid_t<char> _grid;

	void SetUp() override
	{
		grid_t<char> grid(2, 3, {0, 1, 2, 3, 4, 5});

		/*     x=0  x=1  x=2 x=column
		  y=0  0    1    2   y=row
		  y=1  3    4    5         */
		ASSERT_EQ(0, grid(0, 0));
		ASSERT_EQ(1, grid(0, 1));
		ASSERT_EQ(2, grid(0, 2));
		ASSERT_EQ(3, grid(1, 0));
		ASSERT_EQ(4, grid(1, 1));
		ASSERT_EQ(5, grid(1, 2));

		_grid = grid;
	}
};

TEST_F(GridToFromEigenMatrix, grid_convertible_to_matrix)
{
	auto matrix = static_cast<grid_t<char>::container_type&>(_grid);
	ASSERT_EQ(0, matrix(0, 0));
	ASSERT_EQ(1, matrix(0, 1));
	ASSERT_EQ(2, matrix(0, 2));
	ASSERT_EQ(3, matrix(1, 0));
	ASSERT_EQ(4, matrix(1, 1));
	ASSERT_EQ(5, matrix(1, 2));
}

TEST_F(GridToFromEigenMatrix, row_major_matrix_convertible_to_grid)
{
	Matrix<char, 2, 3, RowMajor> matrix;
	matrix << 0, 1, 2, 3, 4, 5;
	ASSERT_EQ(0, matrix(0, 0));
	ASSERT_EQ(1, matrix(0, 1));
	ASSERT_EQ(2, matrix(0, 2));
	ASSERT_EQ(3, matrix(1, 0));
	ASSERT_EQ(4, matrix(1, 1));
	ASSERT_EQ(5, matrix(1, 2));

	_grid = grid_t<char>(matrix);
	ASSERT_EQ(0, _grid(0, 0));
	ASSERT_EQ(1, _grid(0, 1));
	ASSERT_EQ(2, _grid(0, 2));
	ASSERT_EQ(3, _grid(1, 0));
	ASSERT_EQ(4, _grid(1, 1));
	ASSERT_EQ(5, _grid(1, 2));
}

TEST_F(GridToFromEigenMatrix, col_major_matrix_convertible_to_grid)
{
	Matrix<char, 2, 3, ColMajor> matrix;
	matrix << 0, 1, 2, 3, 4, 5;
	ASSERT_EQ(0, matrix(0, 0));
	ASSERT_EQ(1, matrix(0, 1));
	ASSERT_EQ(2, matrix(0, 2));
	ASSERT_EQ(3, matrix(1, 0));
	ASSERT_EQ(4, matrix(1, 1));
	ASSERT_EQ(5, matrix(1, 2));

	_grid = grid_t<char>(matrix);
	ASSERT_EQ(0, _grid(0, 0));
	ASSERT_EQ(1, _grid(0, 1));
	ASSERT_EQ(2, _grid(0, 2));
	ASSERT_EQ(3, _grid(1, 0));
	ASSERT_EQ(4, _grid(1, 1));
	ASSERT_EQ(5, _grid(1, 2));
}

TEST_F(GridToFromEigenMatrix, matrix_is_a_copy_not_a_mutable_view)
{
	auto matrix = _grid.as_matrix();  // not auto&

	auto constexpr mod = 6;
	matrix(0, 0) = mod;
	ASSERT_EQ(mod, matrix(0, 0));
	ASSERT_EQ(0, _grid(0, 0));  // grid is unchanged
}

TEST_F(GridToFromEigenMatrix, matrix_is_a_mutable_view)
{
	auto& matrix = _grid.as_matrix();

	auto constexpr mod = 6;
	matrix(0, 0) = mod;
	ASSERT_EQ(mod, matrix(0, 0));
	ASSERT_EQ(mod, _grid(0, 0));
}

TEST_F(GridToFromEigenMatrix, grid_is_a_copy_not_a_mutable_view)
{
	auto matrix = Matrix<char, 2, 3>();
	matrix << 0, 1, 2, 3, 4, 5;
	_grid = grid_t<char>(matrix);

	auto constexpr mod = 6;
	_grid(0, 0) = mod;
	ASSERT_EQ(mod, _grid(0, 0));
	ASSERT_EQ(0, matrix(0, 0));  // matrix is unchanged
}

// Not in fixture, but still to Eigen matrix
TEST(Grid, as_matrix_const)
{
	grid_t<char> const grid(2, 3, {0, 1, 2, 3, 4, 5});
	auto& matrix = grid.as_matrix();
	ASSERT_EQ(0, matrix(0, 0));
	ASSERT_EQ(1, matrix(0, 1));
	ASSERT_EQ(2, matrix(0, 2));
	ASSERT_EQ(3, matrix(1, 0));
	ASSERT_EQ(4, matrix(1, 1));
	ASSERT_EQ(5, matrix(1, 2));
}
