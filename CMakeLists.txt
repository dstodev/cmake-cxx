cmake_minimum_required(VERSION 3.21)

## Our policies
cmake_policy(SET CMP0091 NEW)  # https://cmake.org/cmake/help/latest/policy/CMP0091.html

## Dependency policies
set(CMAKE_POLICY_DEFAULT_CMP0068 NEW)  # https://cmake.org/cmake/help/latest/policy/CMP0068.html

set(cmake_module_paths
	"${CMAKE_CURRENT_LIST_DIR}/lib/cmake/config"
	"${CMAKE_CURRENT_LIST_DIR}/lib/cmake/include"
)
list(APPEND CMAKE_MODULE_PATH ${cmake_module_paths})  # mostly for include()
list(APPEND CMAKE_PREFIX_PATH ${cmake_module_paths})  # mostly for find_package()

find_package(BuildUtilities CONFIG REQUIRED)

setup_build_mode()

#############
#  Project  #
#############

project(Project)

## Our options
option(TEST_GUI "Run GUI tests (requires attached display)?" TRUE)

## CMake options
set(CMAKE_MESSAGE_LOG_LEVEL "Status" CACHE STRING "CMake message log level")
set_property(CACHE CMAKE_MESSAGE_LOG_LEVEL PROPERTY STRINGS
	"Error" "Warning" "Notice" "Status" "Verbose" "Debug" "Trace"
) # https://cmake.org/cmake/help/latest/manual/cmake.1.html#cmdoption-cmake-log-level

set(CMAKE_CXX_STANDARD 20 CACHE STRING "C++ standard version")

option(BUILD_SHARED_LIBS "Build a shared artifact (.dll, .so, .dylib)?" TRUE)
option(CMAKE_EXPORT_COMPILE_COMMANDS "Generate compile_commands.json?" TRUE)
option(CMAKE_POSITION_INDEPENDENT_CODE "Build position-independent code?" TRUE)
option(CMAKE_TLS_VERIFY "Verify HTTPS server certificates?" TRUE)
option(CMAKE_VERBOSE_MAKEFILE "Emit verbose build output?" FALSE)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL" CACHE STRING "MSVC runtime")

# https://cmake.org/cmake/help/latest/manual/cmake-buildsystem.7.html#output-artifacts
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib" CACHE PATH ".lib, .a, etc.")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib" CACHE PATH ".so, .dylib, etc.")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin" CACHE PATH ".exe, .dll, etc.")

configure_rpath_variables()

include(CheckPIESupported)  # https://cmake.org/cmake/help/latest/module/CheckPIESupported.html
check_pie_supported()

include(dependencies)  # lib/cmake/include/dependencies.cmake

add_subdirectory(src)

if(PROJECT_IS_TOP_LEVEL)
	##############
	#  Testing   #
	##############

	set_property(GLOBAL PROPERTY CTEST_TARGETS_ADDED 1)  # Do not populate CTest/CDash targets
	include(CTest)  # used instead of `enable_testing()` for better IDE compatibility

	add_subdirectory(test)

	#############
	#  Samples  #
	#############

	set(libraries
		project
		SDL2::SDL2main
		SDL2::SDL2 # only for SDL2main
	)
	set(target_name "app-gui") # GUI application
	add_executable(${target_name} WIN32 MACOSX_BUNDLE
		${SAMPLE_MAIN}  # defined in test/sample/targets.cmake
	)
	target_link_libraries(${target_name} PRIVATE ${libraries})

	set(target_name "app-console") # Console application
	add_executable(${target_name} ${SAMPLE_MAIN})
	target_link_libraries(${target_name} PRIVATE ${libraries})

	#############
	#  Package  #
	#############

	add_custom_target(build-package cpack -C $<CONFIG>)
	add_dependencies(build-package
		app-gui
		app-console
	)

	include(package)  # lib/cmake/include/package.cmake
endif()
