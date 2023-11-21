# Look for dependency root adjacent to the build directory
file(REAL_PATH "${PROJECT_BINARY_DIR}/../_dependency-cache" CPM_SOURCE_CACHE)

include(${CMAKE_CURRENT_LIST_DIR}/cpm.cmake)

message(STATUS "Adding dependency packages... (from: ${CPM_SOURCE_CACHE})")

directory_targets(before_dependencies . RECURSIVE)

set(dependency_dir "${CMAKE_CURRENT_LIST_DIR}/dependencies")

include(${dependency_dir}/GoogleTest.cmake)
include(${dependency_dir}/fmt.cmake)

directory_targets(dependency_targets . RECURSIVE)
list(REMOVE_ITEM dependency_targets ${before_dependencies})
log_vars(dependency_targets SPLIT_LISTS)
