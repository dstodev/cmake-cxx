# Look for dependency root adjacent to the build directory
file(REAL_PATH "${PROJECT_BINARY_DIR}/../_dependency-cache" CPM_SOURCE_CACHE)

include(${CMAKE_CURRENT_LIST_DIR}/cpm.cmake)

message(STATUS "Adding dependency packages... (from: ${CPM_SOURCE_CACHE})")

directory_targets(before_dependencies . RECURSIVE)  # record targets before adding dependencies

set(dependency_dir "${CMAKE_CURRENT_LIST_DIR}")

# Include CPack before dependencies to ensure default CPack variables, except what is set here.
include(${dependency_dir}/CPack.cmake)

##################
#  Dependencies  #
##################

# dependency_alias_names and dependency_alias_targets, after this call,
# hold the name of a corresponding cache variable containing the list.
# e.g. Access list name ${dependency_alias_names} and list itself ${${dependency_alias_names}}
record_alias_definitions(dependency_alias_names dependency_alias_targets)

include(${dependency_dir}/GoogleTest.cmake)  # for testing
include(${dependency_dir}/cxxopts.cmake)  # for command line option parsing
include(${dependency_dir}/fmt.cmake)  # for string formatting & logging

##################

directory_targets(dependency_targets . RECURSIVE)  # record targets after adding dependencies
list(REMOVE_ITEM dependency_targets ${before_dependencies})  # remove targets that were already present
list(REMOVE_ITEM dependency_targets ${${dependency_alias_targets}})  # remove raw alias targets

foreach(name target IN ZIP_LISTS ${dependency_alias_names} ${dependency_alias_targets})
	list(APPEND dependency_targets "${name} -> ${target}")
endforeach()

list(SORT dependency_targets)
log_vars(dependency_targets SPLIT_LISTS)  # log remaining dependency targets
