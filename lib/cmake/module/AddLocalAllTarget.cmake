include_guard()

function(add_local_all_target name)
	get_directory_property(targets BUILDSYSTEM_TARGETS)

	string(JOIN " " pretty_targets ${targets})
	message(DEBUG "Adding target ${name} with: ${pretty_targets}")

	foreach(target IN LISTS targets)
		list(APPEND argv "COMMAND;${target}")
	endforeach()

	add_custom_target(${name} ${argv} VERBATIM)
	add_dependencies(${name} ${targets})
endfunction()
