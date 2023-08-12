#include <gtest/gtest.h>

#include <algorithm>
#include <array>
#include <barrier>
#include <iostream>
#include <numeric>

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
			EXPECT_EQ(gauss_sum(tier), count);

			barrier.arrive_and_wait();
			count += (i + 1);
		});
	}

	pool.stop();

	for (auto const& contribution : pool.thread_task_contributions()) {
		EXPECT_EQ(num_tasks / num_threads, contribution);
	}

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
	std::cout << std::endl;
}

TEST(ThreadPool, tasks_run_in_parallel_pass_i_as_argument)
{
	int const num_threads = 40;
	int const num_tasks = num_threads * 100;

	thread_pool_t pool(num_threads);
	std::barrier barrier(num_threads);
	std::atomic_int count = 0;

	auto gauss_sum = [](int n) { return (n * (n + 1)) / 2; };

	for (int i = 0; i < num_tasks; ++i) {
		pool.add_task(
		    [&barrier, &count, gauss_sum](int i) {
			    barrier.arrive_and_wait();
			    int tier = i - i % num_threads;
			    EXPECT_EQ(gauss_sum(tier), count);

			    barrier.arrive_and_wait();
			    count += (i + 1);
		    },
		    i);
	}

	pool.stop();

	for (auto const& contribution : pool.thread_task_contributions()) {
		EXPECT_EQ(num_tasks / num_threads, contribution);
	}

	ASSERT_EQ(gauss_sum(num_tasks), count);
}

TEST(ThreadPool, unconstrained_thread_distribution)
{
	int const num_threads = 40;
	int const num_tasks = num_threads * 100;

	thread_pool_t pool(num_threads);
	std::atomic_int count = 0;

	auto gauss_sum = [](int n) { return (n * (n + 1)) / 2; };

	for (int i = 0; i < num_tasks; ++i) {
		pool.add_task([&count, i]() { count += (i + 1); });
	}

	pool.stop();

	auto const& contributions = pool.thread_task_contributions();
	int const tasks_completed = std::accumulate(contributions.begin(), contributions.end(), 0);
	int const max_contribution = *std::max_element(contributions.begin(), contributions.end());
	int const min_contribution = *std::min_element(contributions.begin(), contributions.end());

	EXPECT_EQ(num_tasks, tasks_completed);

	std::cout << "Threads completed " << tasks_completed << " total tasks:\n";
	std::cout << "  Max contribution: " << max_contribution << "\n";
	std::cout << "  Min contribution: " << min_contribution << "\n";
	std::cout << "  Delta (max - min): " << max_contribution - min_contribution << "\n";

	for (int id = 0; auto const& contribution : contributions) {
		std::cout << "    id " << ++id << ":" << (id > 9 ? " " : "  ") << contribution << "\n";
	}

	std::cout << std::endl;

	ASSERT_EQ(gauss_sum(num_tasks), count);
}

TEST(ThreadPool, multiple_thread_pool_instances)
{
	int const num_pools = 4;
	int const num_threads = 40;
	int const num_tasks = num_threads * 100;

	std::unique_ptr<thread_pool_t<>> pools[num_pools];
	std::unique_ptr<std::barrier<>> barriers[num_pools];
	std::array<std::atomic_int, num_pools> counts;

	for (int i = 0; i < num_pools; ++i) {
		pools[i] = std::make_unique<thread_pool_t<>>(num_threads);
		barriers[i] = std::make_unique<std::barrier<>>(num_threads);
	}

	auto gauss_sum = [](int n) { return (n * (n + 1)) / 2; };

	for (int pool_index = 0; auto& pool : pools) {
		for (int i = 0; i < num_tasks; ++i) {
			pool->add_task([&barriers, &counts, pool_index, i, gauss_sum]() {
				barriers[pool_index]->arrive_and_wait();
				int tier = i - i % num_threads;
				EXPECT_EQ(gauss_sum(tier), counts[pool_index]);

				barriers[pool_index]->arrive_and_wait();
				counts[pool_index] += (i + 1);
			});
		}
		pool_index += 1;
	}

	std::for_each(pools, pools + num_pools, [](auto& pool) { pool->stop(); });
	std::for_each(pools, pools + num_pools, [](auto& pool) {
		for (auto const& contribution : pool->thread_task_contributions()) {
			EXPECT_EQ(num_tasks / num_threads, contribution);
		}
	});
	std::for_each(counts.begin(), counts.end(), [gauss_sum](auto const& count) {
		EXPECT_EQ(gauss_sum(num_tasks), count);
	});
}
