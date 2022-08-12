#ifndef ASSERTIONS_HXX
#define ASSERTIONS_HXX

#include <gtest/gtest.h>
#include <type_traits>

// GoogleTest does not handle type_traits expressions well, so wrap the call
#define MY_ASSERT_SAME_TYPE(expected_type, actual_type) \
	do { /* This is the 'do {} while (0)' macro idiom */ \
		auto predicate = std::is_same_v<expected_type, actual_type>; \
		ASSERT_TRUE(predicate); \
	} while (0)

#endif  // ASSERTIONS_HXX
