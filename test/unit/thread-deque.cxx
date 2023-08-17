#include <gtest/gtest.h>

#include <barrier>

#include <thread-deque.hxx>
#include <thread-pool.hxx>

using namespace project;

TEST(ThreadDeque, construct)
{
	ThreadDeque<int> deque;
	(void) deque;
}

TEST(ThreadDeque, empty)
{
	ThreadDeque<int> deque;
	ASSERT_TRUE(deque.empty());
	deque.emplace_back(1);
	ASSERT_FALSE(deque.empty());
}

TEST(ThreadDeque, size)
{
	ThreadDeque<int> deque;
	ASSERT_EQ(0, deque.size());
	deque.emplace_back(1);
	ASSERT_EQ(1, deque.size());
}

TEST(ThreadDeque, emplace_front)
{
	ThreadDeque<int> deque;
	deque.emplace_front(1);
	ASSERT_EQ(1, deque.front());
}

TEST(ThreadDeque, push_front)
{
	ThreadDeque<int> deque;
	deque.push_front(1);  // rvalue overload
	int const value = deque.front();
	deque.push_front(value);  // lvalue overload
	ASSERT_EQ(1, deque.front());
}

TEST(ThreadDeque, pop_front)
{
	ThreadDeque<int> deque;
	deque.push_front(1);
	ASSERT_EQ(1, deque.pop_front());
	ASSERT_TRUE(deque.empty());
}

TEST(ThreadDeque, front_is_thread_safe)
{
	int const num_threads = 40;
	int const num_tasks = num_threads * 100;

	ThreadDeque<int> deque;
	ThreadPool pool(num_threads);
	std::barrier barrier(num_threads);

	for (int i = 0; i < num_tasks; ++i) {
		pool.add_task([&deque, &barrier]() {
			barrier.arrive_and_wait();
			ASSERT_EQ(1, deque.pop_front());
			deque.emplace_front(1);
			deque.push_front(1);
			ASSERT_EQ(1, deque.front());
		});
	}

	ASSERT_TRUE(deque.empty());

	deque.emplace_front(1);
	pool.stop();

	ASSERT_EQ(num_tasks + 1, deque.size());
}

TEST(ThreadDeque, emplace_back)
{
	ThreadDeque<int> deque;
	deque.emplace_back(1);
	ASSERT_EQ(1, deque.back());
}

TEST(ThreadDeque, push_back)
{
	ThreadDeque<int> deque;
	deque.push_back(1);  // rvalue overload
	int const value = deque.back();
	deque.push_back(value);  // lvalue overload
	ASSERT_EQ(1, deque.back());
}

TEST(ThreadDeque, pop_back)
{
	ThreadDeque<int> deque;
	deque.push_back(1);
	ASSERT_EQ(1, deque.pop_back());
	ASSERT_TRUE(deque.empty());
}

TEST(ThreadDeque, back_is_thread_safe)
{
	int const num_threads = 40;
	int const num_tasks = num_threads * 100;

	ThreadDeque<int> deque;
	ThreadPool pool(num_threads);
	std::barrier barrier(num_threads);

	for (int i = 0; i < num_tasks; ++i) {
		pool.add_task([&deque, &barrier]() {
			barrier.arrive_and_wait();
			ASSERT_EQ(1, deque.pop_back());
			deque.emplace_back(1);
			deque.push_back(1);
			ASSERT_EQ(1, deque.back());
		});
	}

	ASSERT_TRUE(deque.empty());

	deque.emplace_back(1);
	pool.stop();

	ASSERT_EQ(num_tasks + 1, deque.size());
}

TEST(ThreadDeque, deque_is_lifo)
{
	ThreadDeque<int> deque;
	deque.push_front(1);
	deque.push_front(2);
	deque.push_front(3);
	ASSERT_EQ(3, deque.pop_front());
	ASSERT_EQ(2, deque.pop_front());
	ASSERT_EQ(1, deque.pop_front());
}

TEST(ThreadDeque, deque_is_lifo_reverse)
{
	ThreadDeque<int> deque;
	deque.push_back(1);
	deque.push_back(2);
	deque.push_back(3);
	ASSERT_EQ(3, deque.pop_back());
	ASSERT_EQ(2, deque.pop_back());
	ASSERT_EQ(1, deque.pop_back());
}

TEST(ThreadDeque, deque_is_fifo)
{
	ThreadDeque<int> deque;
	deque.push_front(1);
	deque.push_front(2);
	deque.push_front(3);
	ASSERT_EQ(1, deque.pop_back());
	ASSERT_EQ(2, deque.pop_back());
	ASSERT_EQ(3, deque.pop_back());
}

TEST(ThreadDeque, deque_is_fifo_reverse)
{
	ThreadDeque<int> deque;
	deque.push_back(1);
	deque.push_back(2);
	deque.push_back(3);
	ASSERT_EQ(1, deque.pop_front());
	ASSERT_EQ(2, deque.pop_front());
	ASSERT_EQ(3, deque.pop_front());
}
