cmake_minimum_required(VERSION 3.24)

## Modules

set(cmake_module_paths
	"${CMAKE_CURRENT_LIST_DIR}"
	"${CMAKE_CURRENT_LIST_DIR}/lib/cmake/include"
	"${CMAKE_CURRENT_LIST_DIR}/lib/cmake/module"
)
list(APPEND CMAKE_MODULE_PATH ${cmake_module_paths})

include(all_modules)  # lib/cmake/include/all_modules.cmake

## Our policies

message(DEBUG "Setting CMake policies:")
set_policy(CMP0091 NEW)  # https://cmake.org/cmake/help/latest/policy/CMP0091.html
set_policy(CMP0152 NEW)  # https://cmake.org/cmake/help/latest/policy/CMP0152.html

## Dependency policies

set(CMAKE_POLICY_DEFAULT_CMP0068 NEW)  # https://cmake.org/cmake/help/latest/policy/CMP0068.html

#############
#  Project  #
#############

default_build_type("Release")

project(Project VERSION 0.1.0 LANGUAGES CXX)

## Builtin modules

include(CheckPIESupported)  # https://cmake.org/cmake/help/latest/module/CheckPIESupported.html
include(GNUInstallDirs)  # https://cmake.org/cmake/help/latest/module/GNUInstallDirs.html

## Options

set(CMAKE_MESSAGE_LOG_LEVEL "Status" CACHE STRING "CMake message log level")
set_property(CACHE CMAKE_MESSAGE_LOG_LEVEL PROPERTY
	STRINGS "Error" "Warning" "Notice" "Status" "Verbose" "Debug" "Trace")

default_standard(CXX 17)

option(BUILD_SHARED_LIBS "Build a shared artifact (.dll, .so, .dylib)?" TRUE)
option(CMAKE_EXPORT_COMPILE_COMMANDS "Generate compile_commands.json?" TRUE)
option(CMAKE_POSITION_INDEPENDENT_CODE "Build position-independent code?" TRUE)
option(CMAKE_TLS_VERIFY "Verify HTTPS server certificates?" TRUE)
option(CMAKE_VERBOSE_MAKEFILE "Emit verbose build output?" FALSE)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL" CACHE STRING "MSVC runtime")

# https://cmake.org/cmake/help/latest/manual/cmake-buildsystem.7.html#output-artifacts
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}" CACHE PATH ".lib, .a, etc.")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}" CACHE PATH ".so, .dylib, etc.")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}" CACHE PATH ".exe, .dll, etc.")

set(CMAKE_INSTALL_PREFIX "${PROJECT_BINARY_DIR}/install-stage"
	CACHE PATH "When building the `install` target, put artifacts in this directory."
)  # https://cmake.org/cmake/help/latest/variable/CMAKE_INSTALL_PREFIX.html

set(CMAKE_DEBUG_POSTFIX "d" CACHE STRING "Debug artifact postfix")

## Features

configure_rpath_variables()
check_pie_supported()

##################################
#  Dependency & Project Targets  #
##################################

include(all_dependencies)  # lib/cmake/include/all_dependencies.cmake

add_subdirectory(src)

if(PROJECT_IS_TOP_LEVEL)

	#############
	#  Testing  #
	#############

	set_property(GLOBAL PROPERTY CTEST_TARGETS_ADDED 1)  # Do not populate CTest/CDash targets
	include(CTest)  # `include(CTest)` instead of `enable_testing()` for better IDE compatibility

	add_subdirectory(test)

	#############
	#  Samples  #
	#############

	include(sample/targets.cmake)

	highlight_target_output("app-console")

	#############
	#  Package  #
	#############

	add_custom_target(build-package cpack -C $<CONFIG>)
	add_dependencies(build-package
		app-console
	)

	include(package)  # lib/cmake/include/package.cmake
endif()
