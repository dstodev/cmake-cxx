#include <gtest/gtest.h>

#include <notify-assignment.hxx>

using namespace project;

TEST(NotifyAssignment, construct)
{
	NotifyAssignment<int> value;
	ASSERT_EQ(0, value);
}

TEST(NotifyAssignment, assign)
{
	NotifyAssignment<int> value;
	value = 1;
	ASSERT_EQ(1, value);
}

TEST(NotifyAssignment, assign_from_const_instance)
{
	NotifyAssignment<int> value;
	int const copy = 1;
	value = copy;
	ASSERT_EQ(1, value);
}

TEST(NotifyAssignment, observer)
{
	NotifyAssignment<int> value;
	int old_value = -1;
	int new_value = -1;
	value.add_observer([&](auto old, auto new_) {
		old_value = old;
		new_value = new_;
	});
	value = 1;
	ASSERT_EQ(0, old_value);
	ASSERT_EQ(1, new_value);
}

struct NonCopyable
{
	NonCopyable() = default;

	NonCopyable(NonCopyable const& copy) = delete;
	NonCopyable& operator=(NonCopyable const& copy) = delete;

	NonCopyable(NonCopyable&& move) = default;
	NonCopyable& operator=(NonCopyable&& move) = default;

	NonCopyable(int value)
	    : value(value)
	{}

	int value;
};

TEST(NotifyAssignment, assign_non_copyable)
{
	NotifyAssignment<NonCopyable> value;
	auto& as_ref = static_cast<NonCopyable const&>(value);
	ASSERT_EQ(0, as_ref.value);
	value = 1;  // Constructs a NonCopyable rvalue
	ASSERT_EQ(1, as_ref.value);
}

struct NonMovable
{
	NonMovable() = default;

	NonMovable(NonMovable const& copy) = default;
	NonMovable& operator=(NonMovable const& copy) = default;

	NonMovable(NonMovable&& move) = delete;
	NonMovable& operator=(NonMovable&& move) = delete;

	NonMovable(int value)
	    : value(value)
	{}

	int value;
};

TEST(NotifyAssignment, assign_non_movable)
{
	NotifyAssignment<NonMovable> value;
	auto& as_ref = static_cast<NonMovable const&>(value);
	ASSERT_EQ(0, as_ref.value);
	value = 1;
	ASSERT_EQ(1, as_ref.value);
}
