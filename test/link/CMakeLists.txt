find_package(AddGoogleExecutable CONFIG REQUIRED)
find_package(AddLocalAllTarget CONFIG REQUIRED)

find_package(Eigen3 REQUIRED)

add_google_executable(link_gtest TEST
	SOURCES
		link-gtest.cxx
)

add_google_executable(link_benchmark TEST BENCHMARK
	SOURCES
		link-benchmark.cxx
)

add_google_executable(link_eigen TEST
	SOURCES
		link-eigen.cxx

	LIBRARIES
		Eigen3::Eigen
)

add_local_all_target(_all_link)
