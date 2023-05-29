add_custom_target(sample-app
	COMMAND ${CMAKE_COMMAND} -S "${CMAKE_CURRENT_LIST_DIR}" -B "${PROJECT_BINARY_DIR}/sample" -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
	COMMAND ${CMAKE_COMMAND} --build "${PROJECT_BINARY_DIR}/sample" --config ${CMAKE_BUILD_TYPE} --target run
)
add_dependencies(sample-app
	build-package
)

set(SAMPLE_MAIN "${CMAKE_CURRENT_LIST_DIR}/main.cxx" PARENT_SCOPE)
