function(add_local_all_target target_name)
	set(argv "${target_name}")

	get_directory_property(targets BUILDSYSTEM_TARGETS)

	string(JOIN " " pretty_targets ${targets})
	message(STATUS "Adding target ${target_name} with: ${pretty_targets}")

	foreach(target IN LISTS targets)
		list(APPEND argv "COMMAND;${target}")
	endforeach()

	add_custom_target(${argv})
endfunction()
