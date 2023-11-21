set(source_dir "${CMAKE_CURRENT_LIST_DIR}")
set(binary_dir "${PROJECT_BINARY_DIR}/sample")

set(target_name "app-sample")

add_custom_target(${target_name}
	# When using multi-config generators, allow empty CMAKE_BUILD_TYPE (do not pass $<CONFIG> to CMAKE_BUILD_TYPE)
	COMMAND ${CMAKE_COMMAND} -S "${source_dir}" -B "${binary_dir}" -DCMAKE_BUILD_TYPE="${CMAKE_BUILD_TYPE}"
	COMMAND ${CMAKE_COMMAND} --build "${binary_dir}" --config $<CONFIG> --target run --parallel
)
add_dependencies(${target_name} build-package)

set(SAMPLE_MAIN "${CMAKE_CURRENT_LIST_DIR}/main.cxx" PARENT_SCOPE)
