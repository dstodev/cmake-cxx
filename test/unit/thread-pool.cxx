#include <gtest/gtest.h>

#include <algorithm>
#include <array>
#include <barrier>
#include <chrono>
#include <iostream>
#include <numeric>

#include <thread-pool.hxx>

using namespace project;

TEST(ThreadPool, construct)
{
	ThreadPool pool;
}

TEST(ThreadPool, construct_with_four_threads)
{
	ThreadPool pool(4);
}

void do_nothing()
{}

TEST(ThreadPool, add_task)
{
	ThreadPool pool(4);
	pool.add_task(do_nothing);
}

TEST(ThreadPool, add_task_with_lambda)
{
	ThreadPool pool(4);
	pool.add_task([]() {});
}

TEST(ThreadPool, add_task_with_lambda_and_arguments)
{
	ThreadPool<int> pool(4);
	auto future = pool.add_task([](int value) { return value; }, 1);
	ASSERT_EQ(1, future.get());
}

TEST(ThreadPool, stop)
{
	int const num_threads = 40;
	int const num_tasks = num_threads * 100;

	ThreadPool pool(num_threads);
	std::atomic_int count = 0;

	for (int i = 0; i < num_tasks; ++i) {
		pool.add_task([&count]() { count += 1; });
	}

	pool.stop();

	ASSERT_EQ(num_tasks, count);
}

TEST(ThreadPool, wait)
{
	int const num_cycles = 10;
	int const num_threads = 40;
	int const num_tasks = num_threads * 100;

	ThreadPool pool(num_threads);
	std::atomic_uint count = 0;

	for (int cycle = 0; cycle < num_cycles; ++cycle) {
		for (int i = 0; i < num_tasks; ++i) {
			pool.add_task([&count]() { count += 1; });
		}
		pool.wait();
		ASSERT_EQ((cycle + 1) * num_tasks, static_cast<unsigned int>(count));
	}

	ASSERT_EQ(num_cycles * num_tasks, static_cast<unsigned int>(count));
}

TEST(ThreadPool, wait_when_empty)
{
	ThreadPool pool;
	pool.wait();
}

template <typename R = void>
class ThreadTelemetry : public ThreadPool<R>
{
public:
	using BaseType = ThreadPool<R>;

	explicit ThreadTelemetry(unsigned int num_threads = 1, bool deferred = false)
	    : BaseType(num_threads, deferred)
	    , _thread_contributions(num_threads, 0)
	{}

	auto thread_task_contributions() const -> std::vector<unsigned int> const&
	{
		return _thread_contributions;
	}

private:
	void task_completed(unsigned int thread_index) override
	{
		_thread_contributions[thread_index] += 1;
		BaseType::task_completed(thread_index);
	}

	std::vector<unsigned int> _thread_contributions;
};

TEST(ThreadPool, tasks_run_asynchronously)
{
	int const num_threads = 40;
	int const num_tasks = num_threads * 100;

	ThreadTelemetry pool(num_threads);
	std::barrier barrier(num_threads);
	std::atomic_int count = 0;

	auto gauss_sum = [](int n) { return n * (n + 1) / 2; };

	std::vector<int> addends;
	std::mutex mutex;

	for (int i = 0; i < num_tasks; ++i) {
		pool.add_task([&barrier, &count, &gauss_sum, i, &addends, &mutex]() {
			int const tier = i - i % num_threads;
			auto const addend = i + 1;

			barrier.arrive_and_wait();
			EXPECT_EQ(gauss_sum(tier), count);
			barrier.arrive_and_wait();

			count += addend;
			{
				std::lock_guard lock(mutex);
				addends.push_back(addend);
			}
		});
	}

	pool.stop();

	// Each thread is adding a unique number. Assert they are added
	// in undefined order. Make sure at least one number is out of order.
	EXPECT_EQ(num_tasks, addends.size());
	auto out_of_order_count = 0;
	for (int i = 0; i < num_tasks; ++i) {
		if (i + 1 != addends[i]) {
			out_of_order_count += 1;
		}
	}
	EXPECT_GT(out_of_order_count, 0);

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

TEST(ThreadPool, threads_run_concurrently)
{
	int const num_threads = 10;
	int const sleep_for_ms = 10;

	auto const sleep_duration = std::chrono::milliseconds(sleep_for_ms);

	ThreadPool pool(num_threads, true);

	for (int i = 0; i < num_threads; ++i) {
		pool.add_task([=]() { std::this_thread::sleep_for(sleep_duration); });
	}

	auto const before = std::chrono::steady_clock::now();

	pool.start();
	pool.stop();

	auto const after = std::chrono::steady_clock::now();
	auto const elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(after - before).count();

	int const tolerance = (num_threads / 2) * sleep_for_ms;
	EXPECT_LT(elapsed, tolerance);
}

TEST(ThreadPool, tasks_run_in_parallel_pass_i_as_argument)
{
	int const num_threads = 40;
	int const num_tasks = num_threads * 100;

	ThreadTelemetry pool(num_threads);
	std::barrier barrier(num_threads);
	std::atomic_int count = 0;

	for (int i = 0; i < num_tasks; ++i) {
		pool.add_task(
		    [&barrier, &count](int i) {
			    barrier.arrive_and_wait();
			    int tier = i - i % num_threads;
			    EXPECT_EQ(tier, count);

			    barrier.arrive_and_wait();
			    count += 1;
		    },
		    i);
	}

	pool.stop();

	for (auto const& contribution : pool.thread_task_contributions()) {
		EXPECT_EQ(num_tasks / num_threads, contribution);
	}

	ASSERT_EQ(num_tasks, count);
}

TEST(ThreadPool, unconstrained_thread_distribution)
{
	int const num_threads = 40;
	int const num_tasks = num_threads * 100;

	ThreadTelemetry pool(num_threads);
	std::atomic_int count = 0;

	for (int i = 0; i < num_tasks; ++i) {
		pool.add_task([&count]() { count += 1; });
	}

	pool.stop();

	auto const& contributions = pool.thread_task_contributions();
	auto const tasks_completed = std::accumulate(contributions.begin(), contributions.end(), 0u);
	auto const max_contribution = *std::max_element(contributions.begin(), contributions.end());
	auto const min_contribution = *std::min_element(contributions.begin(), contributions.end());

	EXPECT_EQ(num_tasks, tasks_completed);

	std::cout << "Threads completed " << tasks_completed << " total tasks:\n";
	std::cout << "  Max contribution: " << max_contribution << "\n";
	std::cout << "  Min contribution: " << min_contribution << "\n";
	std::cout << "  Delta (max - min): " << max_contribution - min_contribution << "\n";

	for (int id = 0; auto const& contribution : contributions) {
		std::cout << "    id " << id << ":" << (id > 9 ? " " : "  ") << contribution << "\n";
		id += 1;
	}

	std::cout << std::endl;

	ASSERT_EQ(num_tasks, count);
}

TEST(ThreadPool, multiple_thread_pool_instances)
{
	int const num_pools = 4;
	int const num_threads = 40;
	int const num_tasks = num_threads * 100;

	std::unique_ptr<ThreadTelemetry<>> pools[num_pools];
	std::unique_ptr<std::barrier<>> barriers[num_pools];
	std::array<std::atomic_int, num_pools> counts;

	for (int i = 0; i < num_pools; ++i) {
		pools[i] = std::make_unique<ThreadTelemetry<>>(num_threads);
		barriers[i] = std::make_unique<std::barrier<>>(num_threads);
	}

	for (int pool_index = 0; auto& pool : pools) {
		for (int i = 0; i < num_tasks; ++i) {
			pool->add_task([&barriers, &counts, pool_index, i]() {
				barriers[pool_index]->arrive_and_wait();
				int tier = i - i % num_threads;
				EXPECT_EQ(tier, counts[pool_index]);

				barriers[pool_index]->arrive_and_wait();
				counts[pool_index] += 1;
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
	std::for_each(counts.begin(), counts.end(), [&](auto const& count) { EXPECT_EQ(num_tasks, count); });
}

TEST(ThreadPool, deferred_start)
{
	int const num_threads = 40;
	int const num_tasks = num_threads * 100;

	ThreadTelemetry pool(num_threads, true);
	std::atomic_int count = 0;

	for (int i = 0; i < num_tasks; ++i) {
		pool.add_task([&count]() { count += 1; });
	}

	// Give the threads a chance to run
	std::this_thread::sleep_for(std::chrono::milliseconds(1));

	// The threads should not have started yet
	ASSERT_EQ(0, count);

	pool.start();
	pool.stop();

	ASSERT_EQ(num_tasks, count);
}

TEST(ThreadPool, tasks_added_after_start)
{
	int const num_threads = 40;
	int const num_tasks = num_threads * 100;

	ThreadTelemetry pool(num_threads, true);
	std::atomic_int count = 0;

	pool.start();

	for (int i = 0; i < num_tasks; ++i) {
		pool.add_task([&count]() { count += 1; });
	}

	pool.stop();

	ASSERT_EQ(num_tasks, count);
}

TEST(ThreadPool, stop_twice)
{
	ThreadPool pool;
	pool.stop();
	pool.stop();  // should do nothing
}

TEST(ThreadPool, start_in_non_deferred_mode)
{
	ThreadPool pool;
	pool.start();  // should do nothing
}

TEST(ThreadPool, start_twice)
{
	ThreadPool pool(1, true);
	pool.start();
	pool.start();  // should do nothing
}
