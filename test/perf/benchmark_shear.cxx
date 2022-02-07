#include <benchmark/benchmark.h>

#include <shear.hxx>

static void run_shear(benchmark::State& state, float x, float y, float x_mag, float y_mag)
{
	my::point_t point {x, y};

	for (auto _ : state) {
		my::shear(point, x_mag, y_mag);
	}
}

BENCHMARK_CAPTURE(run_shear, "1234", 1.0f, 2.0f, 3.0f, 4.0f);

BENCHMARK_MAIN();
