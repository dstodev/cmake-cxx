#include <gtest/gtest.h>

// #include <placeholder.hxx>  Intentionally left un-included; tests global
//                             include behavior. Kind of meta-testing CMake.

#include <utility>

using namespace project;

TEST(Placeholder, constructor_default)
{
	placeholder_t o;
}

TEST(Placeholder, constructor_copy)
{
	placeholder_t o;
	placeholder_t i(o);
}

TEST(Placeholder, constructor_move)
{
	placeholder_t o(std::move(placeholder_t {}));  // using std::move to avoid copy-elision
}

TEST(Placeholder, constructor_single_int)
{
	placeholder_t o(0);
}

TEST(Placeholder, constructor_two_int)
{
	placeholder_t o(0, 1);
}

TEST(Placeholder, constructor_int_and_char)
{
	placeholder_t o(0, 'a');
}

TEST(Placeholder, operators_assignment)
{
	placeholder_t o, i;
	o = i;  // copy
	o = std::move(i);
}

#define operate(symbol) \
	o symbol i; \
	o symbol 0; \
	o symbol 'a';

/* These tests basically assert that the placeholder type can be used
 * in various situations and still compile.
 */

TEST(Placeholder, operators_arithmetic)
{
	placeholder_t o, i;

	operate(+);
	operate(-);
	operate(*);
	operate(/);
	operate(%);
}

TEST(Placeholder, operators_compound)
{
	placeholder_t o, i;

	operate(+=);
	operate(-=);
	operate(*=);
	operate(/=);
	operate(%=);
}

#undef operate

TEST(Placeholder, operators_comparison)
{
	placeholder_t o, i;

	auto r = o == i;
	r = o != i;
	r = o < i;
	r = o > i;
	r = o <= i;
	r = o >= i;
}

TEST(Placeholder, operator_brackets)
{
	placeholder_t o;
	ASSERT_EQ(o, o["abc"]);
}

TEST(Placeholder, operator_call_empty)
{
	placeholder_t o;
	ASSERT_EQ(o, o());
}

TEST(Placeholder, operator_call)
{
	placeholder_t o;
	ASSERT_EQ(o, o("abc", 123));
}
