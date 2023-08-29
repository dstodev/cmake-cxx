find_package(HelpParseArguments CONFIG REQUIRED)

#[[
	Returns a list (in out_var) of all targets defined in the given directory.
	If RECURSIVE is given, the list will also include targets defined in subdirectories.
]]
function(directory_targets out_var directory)
	help_parse_arguments(args "RECURSIVE" "" "")

	get_property(targets DIRECTORY ${directory} PROPERTY BUILDSYSTEM_TARGETS)

	if(args_RECURSIVE)
		get_property(subdirs DIRECTORY ${directory} PROPERTY SUBDIRECTORIES)

		foreach(subdir IN LISTS subdirs)
			directory_targets(subdir_targets ${subdir} RECURSIVE)
			list(APPEND targets ${subdir_targets})
		endforeach()
	endif()

	set(${out_var} ${targets} PARENT_SCOPE)
endfunction()

#[[
	Returns a list (in out_var) of all targets defined in the same directory as the given target.
	If RECURSIVE is given, the list will also include targets defined in subdirectories.
]]
function(related_targets out_var target)
	get_property(target_dir TARGET ${target} PROPERTY SOURCE_DIR)
	directory_targets(targets ${target_dir} ${ARGN})
	set(${out_var} ${targets} PARENT_SCOPE)
endfunction()
