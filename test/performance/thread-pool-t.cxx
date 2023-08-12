#include <benchmark/benchmark.h>

#include <thread-pool-t.hxx>

using namespace project;

static void count_single_threaded(benchmark::State& state)
{
	int const num_tasks = 4000;

	for (auto _ : state) {
		int count = 0;

		for (int i = 0; i < num_tasks; ++i) {  // loop may be optimized away
			count += (i + 1);
		}

		(void) count;
	}
}

static void count_single_threaded_volatile(benchmark::State& state)
{
	int const num_tasks = 4000;

	for (auto _ : state) {
		volatile int count = 0;  // volatile prevents compiler from optimizing away the loop

		for (int i = 0; i < num_tasks; ++i) {
			count += (i + 1);
		}

		(void) count;
	}
}

static void count_single_threaded_atomic(benchmark::State& state)
{
	int const num_tasks = 4000;

	for (auto _ : state) {
		volatile std::atomic_int count = 0;

		for (int i = 0; i < num_tasks; ++i) {
			count += (i + 1);
		}

		(void) count;
	}
}

static void count_multithreaded(benchmark::State& state)
{
	int const num_threads = 40;
	int const num_tasks = num_threads * 100;

	for (auto _ : state) {
		thread_pool_t pool(num_threads);
		volatile std::atomic_int count = 0;

		for (int i = 0; i < num_tasks; ++i) {
			pool.add_task([&count, i]() { count += (i + 1); });
		}

		(void) count;
	}
}

BENCHMARK(count_single_threaded);
BENCHMARK(count_single_threaded_volatile);
BENCHMARK(count_single_threaded_atomic);
BENCHMARK(count_multithreaded)->Iterations(1000)->Unit(benchmark::kMillisecond);
