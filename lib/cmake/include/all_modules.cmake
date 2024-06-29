set(modules_to_ignore
	"MessageGate"  # could confuse users when automatically included (overrides message())
)

get_filename_component(dir "${CMAKE_CURRENT_LIST_DIR}/../module" REALPATH)

file(GLOB modules CONFIGURE_DEPENDS "${dir}/*.cmake")

foreach(module IN LISTS modules)
	get_filename_component(name "${module}" NAME_WE)

	if(name IN_LIST modules_to_ignore)
		message(DEBUG "Ignoring module: ${name}")
	else()
		message(DEBUG "Including module: ${name}")
		include(${module})
	endif()
endforeach()

unset(dir)
unset(modules)
unset(modules_to_ignore)
unset(name)
