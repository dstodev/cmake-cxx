find_package(HelpParseArguments CONFIG REQUIRED)

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

function(related_targets out_var target)
	get_property(target_dir TARGET ${target} PROPERTY SOURCE_DIR)
	directory_targets(targets ${target_dir} ${ARGN})
	set(${out_var} ${targets} PARENT_SCOPE)
endfunction()
