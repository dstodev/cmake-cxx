#include <gtest/gtest.h>

#include <barrier>
#include <iostream>

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

TEST(ThreadPool, tasks_run_in_parallel)
{
	int const num_threads = 40;
	int const num_tasks = num_threads * 100;

	thread_pool_t pool(num_threads);
	std::barrier barrier(num_threads);
	std::atomic_int count = 0;

	auto gauss_sum = [](int n) { return (n * (n + 1)) / 2; };

	for (int i = 0; i < num_tasks; ++i) {
		pool.add_task([&barrier, &count, i, gauss_sum]() {
			barrier.arrive_and_wait();
			int tier = i - i % num_threads;
			ASSERT_EQ(gauss_sum(tier), count);

			barrier.arrive_and_wait();
			count += (i + 1);
		});
	}

	pool.stop();

	ASSERT_EQ(gauss_sum(num_tasks), count);
}

TEST(ThreadPool, modulo_math)
{
	for (int i = 0; i < 16; ++i) {
		int tier = i - i % 4;
		std::cout << tier << (tier > 9 ? " " : "  ");

		if ((i + 1) % 4 == 0) {
			std::cout << std::endl;
		}
	}
}
