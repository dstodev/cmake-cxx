set(modules_to_ignore
	"MessageGate"  # could confuse users when automatically included (overrides message())
)

file(GLOB modules "${CMAKE_CURRENT_LIST_DIR}/*.cmake")

foreach(module IN LISTS modules)
	get_filename_component(module ${module} REALPATH)
	get_filename_component(this_file ${CMAKE_CURRENT_LIST_FILE} REALPATH)

	if(module PATH_EQUAL this_file)
		continue()
	endif()

	get_filename_component(module ${module} NAME_WE)

	if(module IN_LIST modules_to_ignore)
		message(DEBUG "Ignoring module: ${module}")
	else()
		message(DEBUG "Including module: ${module}")
		include(${module})
	endif()
endforeach()
