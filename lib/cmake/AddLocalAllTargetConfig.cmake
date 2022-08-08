function(add_local_all_target prefix)
	get_directory_property(targets BUILDSYSTEM_TARGETS)
	add_custom_target(${prefix} ${targets})
endfunction()
