cmake_minimum_required(VERSION 3.18)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/lib/cmake")  # mostly for include()
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_LIST_DIR}/lib/cmake")  # mostly for find_package()

find_package(Expect CONFIG REQUIRED)  # some modules use this module
# disable expect() tests by removing the package and defining a simple stub
# replacement function:
#   function(expect)
#   endfunction()
# so modules which call expect() can still be used.

find_package(SetupBuildMode CONFIG REQUIRED)
setup_build_mode("Release")

project(Project)

option(BUILD_SHARED_LIBS "Build a shared artifact (.dll, .so, .dylib)?" TRUE)
option(CMAKE_EXPORT_COMPILE_COMMANDS "Generate compile_commands.json?" TRUE)
option(CMAKE_VERBOSE_MAKEFILE "Emit verbose build output?" FALSE)

# https://cmake.org/cmake/help/latest/manual/cmake-buildsystem.7.html#output-artifacts
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")  # .lib, .a, ...
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")  # .so, .dylib, ...
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")  # .exe, .dll, ...

# When building the `install` target, put artifacts in this directory.
set(CMAKE_INSTALL_PREFIX "${PROJECT_BINARY_DIR}/package_stage")  # https://cmake.org/cmake/help/latest/variable/CMAKE_INSTALL_PREFIX.html

include(CPack)
include(CTest)

# Using Conan integration approach from: https://github.com/conan-io/cmake-conan
include(lib/third-party/conan.cmake)

set(conan_build_dir "${PROJECT_BINARY_DIR}/conan")
list(APPEND CMAKE_MODULE_PATH "${conan_build_dir}")

conan_cmake_autodetect(conan_settings)  # https://github.com/conan-io/cmake-conan#conan_cmake_autodetect
conan_cmake_install(PATH_OR_REFERENCE "${CMAKE_CURRENT_LIST_DIR}/conanfile.txt"  # https://github.com/conan-io/cmake-conan#conan_cmake_install
	BUILD missing
	REMOTE conancenter
	SETTINGS ${conan_settings}
	INSTALL_FOLDER ${conan_build_dir}
)

add_subdirectory(src)
add_subdirectory(test)
