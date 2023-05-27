cmake_minimum_required(VERSION 3.18)

cmake_policy(SET CMP0091 NEW)  # https://cmake.org/cmake/help/latest/policy/CMP0091.html

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/lib/cmake")  # mostly for include()
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_LIST_DIR}/lib/cmake")  # mostly for find_package()

find_package(Expect CONFIG REQUIRED)  # so modules may use expect() as a 'builtin' without find_package()
find_package(LogVars CONFIG REQUIRED)  # For developer utility

find_package(SetupBuildMode CONFIG REQUIRED)
setup_build_mode("Release")

project(Project)

enable_testing()

include(CheckPIESupported)  # https://cmake.org/cmake/help/latest/module/CheckPIESupported.html
check_pie_supported()

option(BUILD_SHARED_LIBS "Build a shared artifact (.dll, .so, .dylib)?" TRUE)
option(CMAKE_EXPORT_COMPILE_COMMANDS "Generate compile_commands.json?" TRUE)
option(CMAKE_POSITION_INDEPENDENT_CODE "Build position-independent code?" TRUE)
option(CMAKE_TLS_VERIFY "Verify HTTPS server certificates?" TRUE)
option(CMAKE_VERBOSE_MAKEFILE "Emit verbose build output?" FALSE)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

# https://cmake.org/cmake/help/latest/manual/cmake-buildsystem.7.html#output-artifacts
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")  # .lib, .a, ...
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")  # .so, .dylib, ...
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")  # .exe, .dll, ...

# When building the `install` target, put artifacts in this directory.
set(CMAKE_INSTALL_PREFIX "${PROJECT_BINARY_DIR}/install-stage")  # https://cmake.org/cmake/help/latest/variable/CMAKE_INSTALL_PREFIX.html

set(CPACK_GENERATOR "ZIP")

include(CPack)
add_custom_target(build-package cpack)

include(dependencies.cmake)

add_subdirectory(src)
add_subdirectory(test)
