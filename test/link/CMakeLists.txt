find_package(AddGoogleExecutable CONFIG REQUIRED)
find_package(AddLocalAllTarget CONFIG REQUIRED)

find_package(Eigen3 REQUIRED)
find_package(OpenCV REQUIRED)

add_google_executable(link_gtest TEST
	SOURCES
		gtest.cxx
)

add_google_executable(link_benchmark TEST BENCHMARK
	SOURCES
		benchmark.cxx
)

add_google_executable(link_dependencies TEST
	SOURCES
		eigen.cxx
		opencv.cxx

	LIBRARIES
		Eigen3::Eigen
		opencv::core
		opencv::imgcodecs
)

add_local_all_target(_all_link)
