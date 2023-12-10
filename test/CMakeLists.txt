add_subdirectory(link)
add_subdirectory(unit)

add_custom_target(all-test)
add_dependencies(all-test
	all-link
	all-unit
)
