find_package(AddGoogleExecutable CONFIG REQUIRED)
find_package(AddLocalAllTarget CONFIG REQUIRED)

add_google_executable(link-gtest TEST
	SOURCES
		gtest.cxx
)

add_google_executable(link-benchmark TEST BENCHMARK
	SOURCES
		benchmark.cxx
)

add_google_executable(link-dependencies TEST
	SOURCES
		eigen.cxx

	LIBRARIES
		Eigen3::Eigen
)

add_local_all_target(_all-link)
