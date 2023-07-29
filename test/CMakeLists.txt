add_subdirectory(integration)
add_subdirectory(link)
add_subdirectory(performance)
add_subdirectory(unit)

add_custom_target(all-test)
add_dependencies(all-test
	all-link
	all-unit
	all-integration
)

include(sample/targets.cmake)

add_subdirectory(flags)
