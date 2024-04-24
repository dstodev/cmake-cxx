# Look for dependency root adjacent to the build directory
file(REAL_PATH "${PROJECT_BINARY_DIR}/../_dependency-cache" CPM_SOURCE_CACHE)

message(STATUS "Adding dependency packages... (from: ${CPM_SOURCE_CACHE})")

set(dependency_dir "${CMAKE_CURRENT_LIST_DIR}")

########################
#  First Dependencies  #
########################

include("${dependency_dir}/first/CPack.cmake")
include("${dependency_dir}/first/cpm.cmake")

########################

directory_targets(before_dependencies . RECURSIVE)  # record targets before adding dependencies

record_alias_definitions(dependency_alias_names dependency_alias_targets)

##################
#  Dependencies  #
##################

include("${dependency_dir}/GoogleTest.cmake")  # for testing
include("${dependency_dir}/cxxopts.cmake")  # for command line option parsing
include("${dependency_dir}/fmt.cmake")  # for string formatting & logging

##################

directory_targets(dependency_targets . RECURSIVE)  # record targets after adding dependencies
list(REMOVE_ITEM dependency_targets ${before_dependencies})  # remove targets that were already present
list(REMOVE_ITEM dependency_targets ${${dependency_alias_targets}})  # remove raw alias targets

foreach(name target IN ZIP_LISTS ${dependency_alias_names} ${dependency_alias_targets})
	list(APPEND dependency_targets "${name} -> ${target}")
endforeach()

list(SORT dependency_targets)
log_vars(dependency_targets MODE DEBUG)  # log remaining dependency targets
