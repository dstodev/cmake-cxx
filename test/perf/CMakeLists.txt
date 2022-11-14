find_package(AddGoogleExecutable CONFIG REQUIRED)
find_package(AddLocalAllTarget CONFIG REQUIRED)

add_google_executable(benchmark_shear BENCHMARK
	SOURCES
		shear.cxx

	LIBRARIES
		project
)

add_local_all_target(all_perf)
