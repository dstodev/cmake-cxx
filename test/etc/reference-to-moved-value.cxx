#include <gtest/gtest.h>

TEST(Etcetera, reference_to_moved_value)
{
	std::string value = "test";
	std::string& ref = value;
	std::string moved = std::move(value);
	// ASSERT_EQ("hello", ref);  // Undefined behavior. ref might be empty, "test", garbage, etc.
	(void) ref;
	ASSERT_EQ("test", moved);
}
