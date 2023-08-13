#include <gtest/gtest.h>

#include <barrier>
#include <iostream>
#include <random>
#include <semaphore>
#include <string>

#include <priority-mutex-t.hxx>
#include <thread-pool-t.hxx>

using namespace project;

namespace {
void jitter_sleep()
{
	std::random_device rd;
	std::mt19937 generator(rd());
	std::uniform_int_distribution distribution(0, 1000);
	std::this_thread::sleep_for(std::chrono::nanoseconds(distribution(generator)));
}
std::string to_string(std::vector<char> const& values)
{
	std::stringstream ss;

	for (auto const& value : values) {
		ss << value << ' ';
	}
	ss << std::endl;

	return ss.str();
}
}  // namespace

TEST(PriorityMutex, construct)
{
	priority_mutex_t mutex;
}

static void TEST_simple_lock(bool high_priority)
{
	priority_mutex_t mutex(high_priority);
	mutex.lock();
	mutex.unlock();
}

TEST(PriorityMutex, low_priority_simple_lock)
{
	TEST_simple_lock(false);
}

TEST(PriorityMutex, high_priority_simple_lock)
{
	TEST_simple_lock(true);
}

static void TEST_lock(bool high_priority)
{
	int const num_threads = 4;
	int const num_tasks = num_threads * 10;
	int const num_additions = 10;

	thread_pool_t pool(num_threads);
	priority_mutex_t mutex(high_priority);
	std::barrier barrier(num_threads);
	volatile int value = 0;

	// Assuming the mutex does not work, try to force a race condition.
	for (int i = 0; i < num_tasks; ++i) {
		pool.add_task([&]() {
			barrier.arrive_and_wait();
			jitter_sleep();
			std::unique_lock lock(mutex);

			for (int j = 0; j < num_additions; ++j) {
				value += 1;
			}
		});
	}

	pool.stop();

	int const expected = num_tasks * num_additions;

	ASSERT_EQ(expected, value) << "Race conditions: " << expected - value;
}

TEST(PriorityMutex, low_priority_lock)
{
	TEST_lock(false);
}

TEST(PriorityMutex, high_priority_lock)
{
	TEST_lock(true);
}

TEST(PriorityMutex, high_priority_lock_takes_priority)
{
	int const num_threads = 10;
	int const num_cycles = 10;

	thread_pool_t pool(num_threads);
	priority_mutex_t mutex(false);
	std::counting_semaphore<num_threads> gate(0);
	std::barrier barrier(num_threads + 1);

	for (int cycles = 0; cycles < num_cycles; ++cycles) {
		std::vector<char> values;

		for (int i = 0; i < num_threads - 1; ++i) {
			pool.add_task([&]() {
				barrier.arrive_and_wait();
				mutex.lock();
				values.push_back('l');
				gate.acquire();
				mutex.unlock();
			});
		}

		pool.add_task([&]() {
			{
				barrier.arrive_and_wait();
				mutex.lock(true);
				values.push_back('h');
				gate.acquire();
				mutex.unlock(true);
			}
		});

		barrier.arrive_and_wait();

		// Give time for threads to reach their mutex locks
		std::this_thread::sleep_for(std::chrono::milliseconds(1));

		gate.release(num_threads);
		pool.wait();

		ASSERT_EQ(num_threads, values.size());

		// The high priority thread is added last, so a low priority thread
		// likely acquires the mutex first, but the high priority thread
		// will always acquire the mutex before another low priority thread.

		if (values[0] == 'h') {
			EXPECT_EQ('l', values[1]) << to_string(values);
		}
		else {
			EXPECT_EQ('l', values[0]) << to_string(values);
			EXPECT_EQ('h', values[1]) << to_string(values);
		}

		for (int i = 2; i < values.size(); ++i) {
			EXPECT_EQ('l', values[i]) << to_string(values);
		}
	}
}
