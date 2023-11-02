# https://github.com/google/benchmark

CPMAddPackage(NAME benchmark
	GITHUB_REPOSITORY google/benchmark
	VERSION 1.8.0
	OPTIONS "BENCHMARK_ENABLE_TESTING OFF"
	EXCLUDE_FROM_ALL TRUE
	SYSTEM TRUE
)
