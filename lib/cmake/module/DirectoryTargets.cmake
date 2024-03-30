include_guard()

include(HelpParseArguments)

#[[
	Returns a list (in out_var) of all targets defined in the given directory.
	Provide RECURSIVE to include targets defined in subdirectories.
]]
function(directory_targets out_var directory)
	help_parse_arguments(args "RECURSIVE" "" "")
	_directory_targets(result "${directory}" ${args_RECURSIVE})
	list(REMOVE_DUPLICATES result)
	set(${out_var} ${result} PARENT_SCOPE)
endfunction()

function(_directory_targets out_var directory recursive)
	get_property(targets DIRECTORY "${directory}" PROPERTY BUILDSYSTEM_TARGETS)
	get_property(imports DIRECTORY "${directory}" PROPERTY IMPORTED_TARGETS)
	list(APPEND targets ${imports})

	if(recursive)
		get_property(subdirs DIRECTORY "${directory}" PROPERTY SUBDIRECTORIES)

		foreach(subdir IN LISTS subdirs)
			_directory_targets(subdir_targets ${subdir} RECURSIVE)
			list(APPEND targets ${subdir_targets})
		endforeach()
	endif()

	set(${out_var} ${targets} PARENT_SCOPE)
endfunction()
