function(add_local_all_target target_name)
	list(APPEND argv "${target_name}")

	get_directory_property(targets BUILDSYSTEM_TARGETS)

	foreach(target IN LISTS targets)
		list(APPEND argv "COMMAND;${target}")
	endforeach()

	add_custom_target(${argv})
endfunction()
