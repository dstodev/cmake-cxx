set(source_dir "${CMAKE_CURRENT_LIST_DIR}")
set(binary_dir "${PROJECT_BINARY_DIR}/sample")

#####################
#  External sample  #
#####################

set(target "app-sample")

add_custom_target(${target}
	COMMAND ${CMAKE_COMMAND} -S "${source_dir}" -B "${binary_dir}" -DCMAKE_BUILD_TYPE="${CMAKE_BUILD_TYPE}"  # --log-level=DEBUG
	COMMAND ${CMAKE_COMMAND} --build "${binary_dir}" --config $<CONFIG> --parallel --target run
	VERBATIM
)
add_dependencies(${target} build-package)

#####################
#  Internal sample  #
#####################

set(target "app-console")

add_executable(${target})

include("${source_dir}/sources.cmake")

target_link_libraries(${target}
	PRIVATE
		project
		cxxopts::cxxopts
)

unset(binary_dir)
unset(source_dir)
unset(target)
