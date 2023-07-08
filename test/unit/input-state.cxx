#include <gtest/gtest.h>

#include <state.hxx>

using namespace project::input;

TEST(State, construct)
{
	State state;
	(void) state;
}

TEST(State, read_virtual)
{
	State state = clone();
	(void) state;
}

TEST(State, update_virtual)
{
	State state;
	update(state);
}
