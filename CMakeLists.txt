cmake_minimum_required(VERSION 3.21)

# Our policies
cmake_policy(SET CMP0091 NEW)  # https://cmake.org/cmake/help/latest/policy/CMP0091.html

# Dependency policies
set(CMAKE_POLICY_DEFAULT_CMP0068 NEW)  # https://cmake.org/cmake/help/latest/policy/CMP0068.html

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/lib/cmake")  # mostly for include()
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_LIST_DIR}/lib/cmake")  # mostly for find_package()

find_package(Expect CONFIG REQUIRED)  # so modules may use expect() as a 'builtin' without find_package()
find_package(BuildUtilities CONFIG REQUIRED)

setup_build_mode()

#############
#  Project  #
#############

project(Project)

set(CMAKE_CXX_STANDARD 20)

option(BUILD_SHARED_LIBS "Build a shared artifact (.dll, .so, .dylib)?" TRUE)
option(CMAKE_EXPORT_COMPILE_COMMANDS "Generate compile_commands.json?" TRUE)
option(CMAKE_POSITION_INDEPENDENT_CODE "Build position-independent code?" TRUE)
option(CMAKE_TLS_VERIFY "Verify HTTPS server certificates?" TRUE)
option(CMAKE_VERBOSE_MAKEFILE "Emit verbose build output?" FALSE)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL" CACHE STRING "MSVC runtime")

# https://cmake.org/cmake/help/latest/manual/cmake-buildsystem.7.html#output-artifacts
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")  # .lib, .a, ...
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")  # .so, .dylib, ...
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")  # .exe, .dll, ...

configure_rpath_variables()

set_property(GLOBAL PROPERTY CTEST_TARGETS_ADDED 1)  # Do not populate CTest/CDash targets
include(CTest)  # This is used instead of `enable_testing()` for better IDE compatibility
#enable_testing()

include(CheckPIESupported)  # https://cmake.org/cmake/help/latest/module/CheckPIESupported.html
check_pie_supported()

set(CMAKE_INSTALL_PREFIX "${PROJECT_BINARY_DIR}/install-stage"
	CACHE PATH "When building the `install` target, put artifacts in this directory."
)  # https://cmake.org/cmake/help/latest/variable/CMAKE_INSTALL_PREFIX.html

include(dependencies.cmake)

add_subdirectory(src)
add_subdirectory(test EXCLUDE_FROM_ALL)

#############
#  Package  #
#############

set(CPACK_GENERATOR "TXZ")
set(CPACK_THREADS 0)  # https://cmake.org/cmake/help/latest/module/CPack.html#variable:CPACK_THREADS
set(CPACK_ARCHIVE_THREADS 0)
set(CPACK_VERBATIM_VARIABLES TRUE)

include(CPack)

add_custom_target(build-package cpack)
add_dependencies(build-package project)

#############
#  Samples  #
#############

# Configure sample applications
set(libraries
	project
	SDL2::SDL2main
	SDL2::SDL2 # only for SDL2main
)
set(target_name "app-gui") # GUI application
add_executable(${target_name} WIN32 MACOSX_BUNDLE
	${SAMPLE_MAIN}  # Defined under `test` subdirectory
)
target_link_libraries(${target_name} PRIVATE ${libraries})

set(target_name "app-console") # Console application
add_executable(${target_name} ${SAMPLE_MAIN})
target_link_libraries(${target_name} PRIVATE ${libraries})

include(export.cmake)
