# Look for dependency root adjacent to the build directory
set(CPM_SOURCE_CACHE "${PROJECT_BINARY_DIR}/../_dependency-cache")

include(lib/third-party/cpm.cmake)

CPMAddPackage(NAME googletest
	GITHUB_REPOSITORY google/googletest
	VERSION 1.13.0
	OPTIONS "INSTALL_GTEST OFF" "gtest_force_shared_crt"
	EXCLUDE_FROM_ALL TRUE
	SYSTEM TRUE
)
CPMAddPackage(NAME benchmark
	GITHUB_REPOSITORY google/benchmark
	VERSION 1.8.0
	OPTIONS "BENCHMARK_ENABLE_TESTING OFF"
	EXCLUDE_FROM_ALL TRUE
	SYSTEM TRUE
)
CPMAddPackage(NAME eigen
	GITLAB_REPOSITORY libeigen/eigen
	GIT_TAG 3.4.0
	DOWNLOAD_ONLY TRUE
)
add_library(eigen INTERFACE)
add_library(Eigen3::Eigen ALIAS eigen)
target_include_directories(eigen
	INTERFACE
		$<BUILD_INTERFACE:${eigen_SOURCE_DIR}>
		$<INSTALL_INTERFACE:include>
)
CPMAddPackage(NAME sdl
	GITHUB_REPOSITORY libsdl-org/SDL
	GIT_TAG release-2.26.5
	OPTIONS "SDL2_DISABLE_INSTALL ON" "SDL2_DISABLE_UNINSTALL ON" "SDL_TEST OFF"
	EXCLUDE_FROM_ALL TRUE
	SYSTEM TRUE
)
CPMAddPackage(NAME fmt
	GITHUB_REPOSITORY fmtlib/fmt
	GIT_TAG 10.0.0
	OPTIONS "FMT_TEST OFF" "FMT_DOC OFF" "FMT_INSTALL OFF"
	EXCLUDE_FROM_ALL TRUE
	SYSTEM TRUE
)
set_target_properties(fmt PROPERTIES PUBLIC_HEADER "")
