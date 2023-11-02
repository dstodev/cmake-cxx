# Look for dependency root adjacent to the build directory
file(REAL_PATH "${PROJECT_BINARY_DIR}/../_dependency-cache" CPM_SOURCE_CACHE)

if(BUILD_SHARED_LIBS)
	set(lib_type SHARED)
else()
	set(lib_type STATIC)
endif()

include(${CMAKE_CURRENT_LIST_DIR}/cpm.cmake)

message(STATUS "Adding dependency packages... (from: ${CPM_SOURCE_CACHE})")

directory_targets(before_dependencies . RECURSIVE)

set(dependency_dir "${CMAKE_CURRENT_LIST_DIR}/dependencies")

include(${dependency_dir}/Eigen.cmake)
include(${dependency_dir}/GLEW.cmake)
include(${dependency_dir}/GoogleBenchmark.cmake)
include(${dependency_dir}/GoogleTest.cmake)
include(${dependency_dir}/OpenGL.cmake)
include(${dependency_dir}/SDL2.cmake)
include(${dependency_dir}/fmt.cmake)

directory_targets(dependency_targets . RECURSIVE)
list(REMOVE_ITEM dependency_targets ${before_dependencies})
log_vars(dependency_targets SPLIT_LISTS)
