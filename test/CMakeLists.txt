add_subdirectory(link)
add_subdirectory(perf)
add_subdirectory(unit)

add_custom_target(all_test)
add_dependencies(all_test
	_all_link
	_all_unit
)
