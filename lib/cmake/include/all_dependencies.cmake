# Look for dependency root adjacent to the build directory
file(REAL_PATH "${PROJECT_BINARY_DIR}/../_dependency-cache" CPM_SOURCE_CACHE)

message(STATUS "Adding dependency packages... (from: ${CPM_SOURCE_CACHE})")

get_filename_component(dir "${PROJECT_SOURCE_DIR}/dependencies" REALPATH)

########################
#  First Dependencies  #
########################

include("${dir}/first/CPack.cmake")
include("${dir}/first/cpm.cmake")

########################

directory_targets(before_dependencies . RECURSIVE)  # record targets before adding dependencies

record_alias_definitions(dependency_alias_names dependency_alias_targets)

##################
#  Dependencies  #
##################

file(GLOB unordered_dependencies CONFIGURE_DEPENDS "${dir}/unordered/*.cmake")

foreach(file ${unordered_dependencies})
	get_filename_component(name "${file}" NAME)
	message(DEBUG "Including dependency stub: ${name}")
	include("${file}")
endforeach()

##################

directory_targets(dependency_targets . RECURSIVE)  # record targets after adding dependencies
list(REMOVE_ITEM dependency_targets ${before_dependencies})  # remove targets that were already present
list(REMOVE_ITEM dependency_targets ${${dependency_alias_targets}})  # remove raw alias targets

foreach(name target IN ZIP_LISTS ${dependency_alias_names} ${dependency_alias_targets})
	list(APPEND dependency_targets "${name} -> ${target}")
endforeach()

list(SORT dependency_targets)
log_vars(dependency_targets MODE DEBUG)  # log remaining dependency targets

unset(before_dependencies)
unset(dependency_alias_names)
unset(dependency_alias_targets)
unset(dependency_targets)
unset(dir)
unset(name)
unset(target)
unset(unordered_dependencies)
