find_package(AddGoogleExecutable CONFIG REQUIRED)

add_google_executable(benchmark_shear BENCHMARK
	SOURCES
		perf-shear.cxx

	LIBRARIES
		shear
)

add_custom_target(all_benchmarks
	benchmark_shear
)
