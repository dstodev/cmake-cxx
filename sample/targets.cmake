set(source_dir "${CMAKE_CURRENT_LIST_DIR}")
set(binary_dir "${PROJECT_BINARY_DIR}/sample")

#####################
#  External sample  #
#####################
set(target_name "app-sample")

add_custom_target(${target_name}
	COMMAND ${CMAKE_COMMAND} -S "${source_dir}" -B "${binary_dir}" -DCMAKE_BUILD_TYPE="${CMAKE_BUILD_TYPE}"
	COMMAND ${CMAKE_COMMAND} --build "${binary_dir}" --config $<CONFIG> --parallel --target run
)
add_dependencies(${target_name} build-package)

#####################
#  Internal sample  #
#####################
set(target_name "app-console")

add_executable(${target_name}
	${source_dir}/main.cxx

	${source_dir}/cli.cxx
	${source_dir}/cli.hxx
)
target_include_directories(${target_name}
	PRIVATE
		${source_dir}
)
target_link_libraries(${target_name}
	PRIVATE
		project
		cxxopts::cxxopts
)
