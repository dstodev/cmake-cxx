add_google_executable(_benchmark BENCHMARK
	SOURCES
		shear.cxx

	LIBRARIES
		project

		Eigen3::Eigen
)

add_local_all_target(all-performance)
