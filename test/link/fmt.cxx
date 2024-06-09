#include <gtest/gtest.h>

#include <fmt/core.h>

TEST(Link, fmt)
{
	fmt::format_string<char> const o {""};
	(void) o;
}
