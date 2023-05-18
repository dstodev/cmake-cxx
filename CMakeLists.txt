cmake_minimum_required(VERSION 3.18)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/lib/cmake")  # mostly for include()
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_LIST_DIR}/lib/cmake")  # mostly for find_package()

find_package(Expect CONFIG REQUIRED)  # some modules use this module

find_package(SetupBuildMode CONFIG REQUIRED)
setup_build_mode("Release")

expect(CMAKE_BUILD_TYPE MATCHES "Debug|Release")

project(Project)

enable_testing()

option(BUILD_SHARED_LIBS "Build a shared artifact (.dll, .so, .dylib)?" TRUE)
option(CMAKE_EXPORT_COMPILE_COMMANDS "Generate compile_commands.json?" TRUE)
option(CMAKE_VERBOSE_MAKEFILE "Emit verbose build output?" FALSE)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

# https://cmake.org/cmake/help/latest/manual/cmake-buildsystem.7.html#output-artifacts
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")  # .lib, .a, ...
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")  # .so, .dylib, ...
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")  # .exe, .dll, ...

# When building the `install` target, put artifacts in this directory.
set(CMAKE_INSTALL_PREFIX "${PROJECT_BINARY_DIR}/package-stage")  # https://cmake.org/cmake/help/latest/variable/CMAKE_INSTALL_PREFIX.html

include(CPack)

set(CPM_SOURCE_CACHE "${PROJECT_BINARY_DIR}/../dependency-cache")

include(lib/third-party/cpm.cmake)

CPMAddPackage(NAME googletest
	GITHUB_REPOSITORY google/googletest
	VERSION 1.13.0
	OPTIONS "INSTALL_GTEST OFF" "gtest_force_shared_crt"
)

CPMAddPackage("gh:google/benchmark@1.8.0")  # Must come after googletest

CPMAddPackage(NAME eigen
	GITLAB_REPOSITORY libeigen/eigen
	GIT_TAG 3.4.0
	OPTIONS "BUILD_TESTING OFF" "EIGEN_BUILD_DOC OFF"
)

add_subdirectory(src)
add_subdirectory(test)
