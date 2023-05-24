add_subdirectory(link)
add_subdirectory(perf)
add_subdirectory(unit)

add_custom_target(all-test)
add_dependencies(all-test
	_all-link
	_all-unit
)

add_custom_target(sample-app
	COMMAND ${CMAKE_COMMAND} -S "${CMAKE_CURRENT_LIST_DIR}/sample" -B "${PROJECT_BINARY_DIR}/sample"
	COMMAND ${CMAKE_COMMAND} --build "${PROJECT_BINARY_DIR}/sample" --target run
)
add_dependencies(sample-app
	build-package
)
