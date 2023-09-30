# Look for dependency root adjacent to the build directory
file(REAL_PATH "${PROJECT_BINARY_DIR}/../_dependency-cache" CPM_SOURCE_CACHE)

if(BUILD_SHARED_LIBS)
	set(lib_type SHARED)
else()
	set(lib_type STATIC)
endif()

include(lib/third-party/cpm.cmake)

message(STATUS "Adding system packages...")

find_package(OpenGL REQUIRED)

message(STATUS "Adding dependency packages...\n   (from: ${CPM_SOURCE_CACHE})")

message_gate_close()

##  GoogleTest  ##
CPMAddPackage(NAME googletest
	GITHUB_REPOSITORY google/googletest
	VERSION 1.13.0
	OPTIONS "BUILD_GMOCK OFF" "INSTALL_GTEST OFF" "gtest_force_shared_crt"
	EXCLUDE_FROM_ALL TRUE
	SYSTEM TRUE
)
directory_targets(targets ${googletest_SOURCE_DIR} RECURSIVE)
string(REPLACE ";" " " targets "${targets}")
message(FORCE STATUS "Found Googletest: ${targets}")

##  Google Benchmark  ##
CPMAddPackage(NAME benchmark
	GITHUB_REPOSITORY google/benchmark
	VERSION 1.8.0
	OPTIONS "BENCHMARK_ENABLE_TESTING OFF"
	EXCLUDE_FROM_ALL TRUE
	SYSTEM TRUE
)
directory_targets(targets ${benchmark_SOURCE_DIR} RECURSIVE)
string(REPLACE ";" " " targets "${targets}")
message(FORCE STATUS "Found Benchmark: ${targets}")

##  Eigen 3  ##
CPMAddPackage(NAME eigen
	GITLAB_REPOSITORY libeigen/eigen
	GIT_TAG 3.4.0
	DOWNLOAD_ONLY TRUE
)
add_library(eigen INTERFACE)
add_library(Eigen3::Eigen ALIAS eigen)
target_include_directories(eigen INTERFACE
	$<BUILD_INTERFACE:${eigen_SOURCE_DIR}>
	$<INSTALL_INTERFACE:include>
)
set(eigen_SOURCE_DIR ${CPM_PACKAGE_eigen_SOURCE_DIR} CACHE INTERNAL "Eigen source directory")
message(FORCE STATUS "Found Eigen: eigen Eigen3::Eigen")

##  SDL2  ##
CPMAddPackage(NAME sdl
	GITHUB_REPOSITORY libsdl-org/SDL
	GIT_TAG release-2.26.5
	OPTIONS "SDL2_DISABLE_INSTALL ON" "SDL2_DISABLE_UNINSTALL ON" "SDL_TEST OFF"
	EXCLUDE_FROM_ALL TRUE
	SYSTEM TRUE
)
directory_targets(targets ${sdl_SOURCE_DIR} RECURSIVE)
string(REPLACE ";" " " targets "${targets}")
message(FORCE STATUS "Found SDL: ${targets}")

##  fmt  ##
CPMAddPackage(NAME fmt
	GITHUB_REPOSITORY fmtlib/fmt
	GIT_TAG 10.0.0
	OPTIONS "FMT_TEST OFF" "FMT_DOC OFF" "FMT_INSTALL OFF"
	EXCLUDE_FROM_ALL TRUE
	SYSTEM TRUE
)
set_target_properties(fmt PROPERTIES PUBLIC_HEADER "")
directory_targets(targets ${fmt_SOURCE_DIR} RECURSIVE)
string(REPLACE ";" " " targets "${targets}")
message(FORCE STATUS "Found fmt: ${targets}")

message_gate_open()  # This should be at bottom of file

##  GLEW  ##
if(WIN32)
	# https://github.com/nigels-com/glew/releases
	set(url "https://github.com/nigels-com/glew/releases/download/glew-2.2.0/glew-2.2.0-win32.zip")
	# set(url "https://github.com/nigels-com/glew/releases/download/glew-2.2.0/glew-2.2.0.tgz")

	CPMAddPackage(NAME glew
		URL ${url}
		DOWNLOAD_ONLY TRUE
	)
	set(target_name GLEW::GLEW)

	add_library(${target_name} IMPORTED ${lib_type})
	if(BUILD_SHARED_LIBS)
		set_target_properties(${target_name} PROPERTIES
			IMPORTED_LOCATION "${glew_SOURCE_DIR}/bin/Release/x64/glew32.dll"
			IMPORTED_IMPLIB "${glew_SOURCE_DIR}/lib/Release/x64/glew32.lib"
		)
		set(runtime_output_directory "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
		configure_file("${glew_SOURCE_DIR}/bin/Release/x64/glew32.dll" "${runtime_output_directory}/glew32.dll" COPYONLY)
	else()
		set_target_properties(${target_name} PROPERTIES
			IMPORTED_LOCATION "${glew_SOURCE_DIR}/lib/Release/x64/glew32s.lib"
		)
	endif()
	target_include_directories(${target_name} INTERFACE
		$<BUILD_INTERFACE:${glew_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
	)
	message(FORCE STATUS "Found GLEW: ${target_name}")
elseif(APPLE)
	find_package(GLEW)  # REQUIRED keyword is not respected by FindGLEW.cmake
	if(GLEW_FOUND)
		message(FORCE STATUS "Found GLEW: GLEW::GLEW")
	else()
		message(FATAL_ERROR " Could not find GLEW! Please install it e.g. brew install glew")
	endif()
endif()
