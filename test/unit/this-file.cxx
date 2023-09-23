#include <gtest/gtest.h>

#include <this-file.hxx>

TEST(ThisFile, THIS_FILE)
{
	std::filesystem::path const path = THIS_FILE();
	auto const expected = std::filesystem::path {__FILE__};
	ASSERT_EQ(expected, path);
}

TEST(ThisFile, THIS_DIRECTORY)
{
	std::filesystem::path const path = THIS_DIRECTORY();
	auto const expected = std::filesystem::path {__FILE__}.parent_path();
	ASSERT_EQ(expected, path);
}
