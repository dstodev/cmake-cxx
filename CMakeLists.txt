cmake_minimum_required(VERSION 3.16)
project(EigenSample)

if (NOT CMAKE_BUILD_TYPE)
	#[[
		If the build is invoked from the command line e.g.:
			cmake -B build_dir -S source_dir
		and CMAKE_BUILD_TYPE is left unspecified, then it will be set in
		the CMake cache as an empty string.
		So we use FORCE here to overwrite it to a better default.
		(and Conan requires that it is NOT an empty string!)
	]]
	set(CMAKE_BUILD_TYPE Release CACHE STRING "Build mode (Debug, Release, ...)" FORCE)
endif()

option(BUILD_SHARED_LIBS "Build a shared artifact (.dll, .so, .dylib)?" TRUE)
option(CMAKE_EXPORT_COMPILE_COMMANDS "Generate compile_commands.json?" TRUE)

# https://cmake.org/cmake/help/latest/manual/cmake-buildsystem.7.html#output-artifacts
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")  # .exe, .dll, ...
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")  # .so, .dylib, ...
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")  # .lib, .a, ...

list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_LIST_DIR}/lib/cmake")

include(CPack)
include(CTest)

# Using Conan integration approach from: https://github.com/conan-io/cmake-conan
include(lib/third_party/conan.cmake)

conan_cmake_autodetect(conan_settings)  # https://github.com/conan-io/cmake-conan#conan_cmake_autodetect
conan_cmake_install(PATH_OR_REFERENCE "${CMAKE_CURRENT_LIST_DIR}/conanfile.txt"  # https://github.com/conan-io/cmake-conan#conan_cmake_install
    BUILD missing
    REMOTE conancenter
    SETTINGS ${conan_settings}
)

add_subdirectory(src)
add_subdirectory(test)
