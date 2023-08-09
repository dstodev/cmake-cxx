#include <gtest/gtest.h>

#include <thread-pool-t.hxx>

using namespace project;

TEST(ThreadPool, construct)
{
	thread_pool_t pool;
}

TEST(ThreadPool, construct_with_four_threads)
{
	thread_pool_t pool(4);
}

void do_nothing()
{}

TEST(ThreadPool, add_task)
{
	thread_pool_t pool(4);
	pool.add_task(do_nothing);
}

TEST(ThreadPool, add_task_with_lambda)
{
	thread_pool_t pool(4);
	pool.add_task([]() {});
}

TEST(ThreadPool, add_task_with_lambda_and_arguments)
{
	thread_pool_t<int> pool(4);
	auto future = pool.add_task([](int value) { return value; }, 1);
	ASSERT_EQ(1, future.get());
}
