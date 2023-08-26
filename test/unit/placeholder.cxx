#include <gtest/gtest.h>

// #include <placeholder.hxx>  Intentionally left un-included; tests global
//                             include behavior. Kind of meta-testing CMake.

#include <utility>

TEST(Placeholder, constructor_default)
{
	Placeholder o;
}

TEST(Placeholder, constructor_copy)
{
	Placeholder o;
	Placeholder i(o);
}

TEST(Placeholder, constructor_move)
{
	Placeholder o(std::move(Placeholder {}));  // using std::move to avoid copy-elision
}

TEST(Placeholder, constructor_single_int)
{
	Placeholder o(0);
}

TEST(Placeholder, constructor_two_int)
{
	Placeholder o(0, 1);
}

TEST(Placeholder, constructor_int_and_char)
{
	Placeholder o(0, 'a');
}

TEST(Placeholder, operators_assignment)
{
	Placeholder o, i;
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
	Placeholder o, i;
	operate(+);
	operate(-);
	operate(*);
	operate(/);
	operate(%);
}

TEST(Placeholder, operators_compound)
{
	Placeholder o, i;
	operate(+=);
	operate(-=);
	operate(*=);
	operate(/=);
	operate(%=);
}

#undef operate

TEST(Placeholder, operators_comparison)
{
	Placeholder o, i;
	ASSERT_TRUE(o == i);
	ASSERT_FALSE(o != i);
	ASSERT_FALSE(o < i);
	ASSERT_FALSE(o > i);
	ASSERT_TRUE(o <= i);
	ASSERT_TRUE(o >= i);
}

TEST(Placeholder, operator_brackets)
{
	Placeholder o;
	ASSERT_EQ(o, o["abc"]);
}

TEST(Placeholder, operator_call_empty)
{
	Placeholder o;
	ASSERT_EQ(o, o());
}

TEST(Placeholder, operator_call)
{
	Placeholder o;
	ASSERT_EQ(o, o("abc", 123));
}
