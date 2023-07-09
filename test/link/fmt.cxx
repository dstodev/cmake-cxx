#include <fmt/core.h>
#include <gtest/gtest.h>

TEST(Link, fmt)
{
	fmt::format_string<char> o {""};
	(void) o;
}
