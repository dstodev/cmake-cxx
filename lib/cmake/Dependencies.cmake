set(CPM_SOURCE_CACHE "${PROJECT_BINARY_DIR}/../dependency-cache")

include(lib/third-party/cpm.cmake)

CPMAddPackage(NAME googletest
	GITHUB_REPOSITORY google/googletest
	VERSION 1.13.0
	OPTIONS "INSTALL_GTEST OFF" "gtest_force_shared_crt"
	EXCLUDE_FROM_ALL TRUE
	SYSTEM TRUE)

CPMAddPackage(NAME benchmark
	GITHUB_REPOSITORY google/benchmark
	VERSION 1.8.0
	OPTIONS "BENCHMARK_ENABLE_TESTING OFF"
	EXCLUDE_FROM_ALL TRUE
	SYSTEM TRUE)

CPMAddPackage(NAME eigen
	GITLAB_REPOSITORY libeigen/eigen
	GIT_TAG 3.4.0
	DOWNLOAD_ONLY TRUE)

add_library(Eigen3::Eigen INTERFACE IMPORTED)
target_include_directories(Eigen3::Eigen
	INTERFACE
		"${eigen_SOURCE_DIR}"
)
