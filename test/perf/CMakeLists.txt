find_package(AddGoogleExecutable REQUIRED)

add_google_executable(benchmark_shear BENCHMARK
	SOURCES
		benchmark_shear.cxx

	LIBRARIES
		shear
)
