#include <benchmark/benchmark.h>

#include <shear.hxx>

using namespace project;

static void run_shear(benchmark::State& state, float x, float y, float x_mag, float y_mag)
{
	point_t point {x, y};

	for (auto _ : state) {
		shear(point, x_mag, y_mag);
	}
}

BENCHMARK_CAPTURE(run_shear, 1234, 1.0f, 2.0f, 3.0f, 4.0f);
