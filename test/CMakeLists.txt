add_subdirectory(link)
add_subdirectory(perf)
add_subdirectory(unit)

add_custom_target(all-test)
add_dependencies(all-test
	_all-link
	_all-unit
)
