find_package(AddGoogleExecutable CONFIG REQUIRED)
find_package(AddLocalAllTarget CONFIG REQUIRED)

add_google_executable(performance BENCHMARK
	SOURCES
		shear.cxx

	LIBRARIES
		project
		Eigen3::Eigen
)

add_local_all_target(all-perf)
