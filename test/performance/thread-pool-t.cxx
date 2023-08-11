#include <benchmark/benchmark.h>

#include <thread-pool-t.hxx>

using namespace project;

static void run_threads(benchmark::State& state)
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

BENCHMARK(run_threads)->Iterations(1000)->Unit(benchmark::kMillisecond);
