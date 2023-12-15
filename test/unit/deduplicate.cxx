#include <gtest/gtest.h>

#include <deduplicate.hxx>

using namespace project;

TEST(StableDeduplicate, empty_vector)
{
	std::vector<std::string> const input {};
	auto const result = stable_deduplicate(input);
	ASSERT_TRUE(result.empty());
}

TEST(StableDeduplicate, one_element)
{
	std::vector<std::string> const input {"a"};
	auto const result = stable_deduplicate(input);
	ASSERT_EQ(result.size(), 1);
	ASSERT_EQ("a", result[0]);
}

TEST(StableDeduplicate, two_elements_in_lexicographic_order)
{
	std::vector<std::string> const input {"a", "b"};
	auto const result = stable_deduplicate(input);
	ASSERT_EQ(result.size(), 2);
	ASSERT_EQ("a", result[0]);
	ASSERT_EQ("b", result[1]);
}

TEST(StableDeduplicate, two_elements_are_not_sorted)
{
	std::vector<std::string> const input {"b", "a"};
	auto const result = stable_deduplicate(input);
	ASSERT_EQ(result.size(), 2);
	ASSERT_EQ("b", result[0]);
	ASSERT_EQ("a", result[1]);
}

TEST(StableDeduplicate, two_elements_are_equal)
{
	std::vector<std::string> const input {"a", "a"};
	auto const result = stable_deduplicate(input);
	ASSERT_EQ(result.size(), 1);
	ASSERT_EQ("a", result[0]);
}

TEST(StableDeduplicate, deduplicated_elements_perserve_order)
{
	std::vector<std::string> const input {"b", "b", "c", "a", "a", "b", "c"};
	auto const result = stable_deduplicate(input);
	ASSERT_EQ(result.size(), 3);
	ASSERT_EQ("b", result[0]);
	ASSERT_EQ("c", result[1]);
	ASSERT_EQ("a", result[2]);
}
