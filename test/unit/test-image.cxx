#include <gtest/gtest.h>

#include <image.hxx>
#include <shear.hxx>

#include <string>

using namespace project;

TEST(Image, construct)
{
	Image<char> image;
	ASSERT_EQ(0, image.width());
	ASSERT_EQ(0, image.height());
}

TEST(Image, construct_non_default)
{
	Image<char> image(0, 1);
	ASSERT_EQ(0, image.width());
	ASSERT_EQ(1, image.height());
}

TEST(Image, construct_copy)
{
	Image<char> image(0, 1);
	Image<char> copy(image);  // NOLINT(performance-unnecessary-copy-initialization)
	ASSERT_EQ(0, copy.width());
	ASSERT_EQ(1, copy.height());
}

TEST(Image, setters)
{
	Image<char> image(0, 0);
	image.width(1);
	image.height(2);
	ASSERT_EQ(1, image.width());
	ASSERT_EQ(2, image.height());

	auto& pixels = image.pixels();
	ASSERT_TRUE(pixels.empty());

	image.pixels(decltype(image)::container_type {3, 4});
	ASSERT_EQ(3, pixels[0]);
	ASSERT_EQ(4, pixels[1]);
}

TEST(Image, construct_with_invalid_data_dimensions)
{
	ASSERT_ANY_THROW(Image<char> image(0, 0, {1}));
}

TEST(Image, construct_with_data)
{
	Image<char> image(1, 2, {3, 4});
	auto& pixels = image.pixels();
	ASSERT_EQ(3, pixels[0]);
	ASSERT_EQ(4, pixels[1]);
}

TEST(Image, pixel_horizontal_rectangle)
{
	/*
	      0 1 2 3 4   a b c d e f g h i j
	    0 a b c d e   0 1 2 3 4 5 6 7 8 9
	    1 f g h i j
	 */
	Image<char> image(5, 2, {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j'});
	ASSERT_EQ('h', image.pixel(1, 2)) << image;
}

TEST(Image, pixel_vertical_rectangle)
{
	/*
	      0 1   a b c d e f g h i j
	    0 a b   0 1 2 3 4 5 6 7 8 9
	    1 c d
	    2 e f
	    3 g h
	    4 i j
	 */
	Image<char> image(2, 5, {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j'});
	ASSERT_EQ('f', image.pixel(2, 1)) << image;
}

TEST(Image, pixel_row_out_of_bounds)
{
	Image<char> image(1, 1);
	image.pixel(0, 0);
	ASSERT_ANY_THROW(image.pixel(1, 0)) << image;
}

TEST(Image, pixel_column_out_of_bounds)
{
	Image<char> image(1, 1);
	image.pixel(0, 0);
	ASSERT_ANY_THROW(image.pixel(0, 1)) << image;
}

TEST(Image, iterator)
{
	Image<char> image(2, 2, {0, 1, 2, 3});
	for (auto i : image) {
		ASSERT_EQ(i, image.pixels()[i]);
	}
}

TEST(Image, iterator_mutates)
{
	Image<char> image(2, 2, {0, 1, 2, 3});
	for (auto& i : image) {
		auto index = i;
		i += 1;
		ASSERT_EQ(index + 1, image.pixels()[index]);
	}
}

TEST(Image, const_iterator)
{
	Image<char> const image(2, 2, {0, 1, 2, 3});
	for (auto i : image) {
		ASSERT_EQ(i, image.pixels()[i]);
	}
}
