target_sources(${target_name}
	PRIVATE
		factory.cxx
		factory.hxx
		program.cxx
		program.hxx
)
target_include_directories(${target_name}
	PRIVATE
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
)

add_subdirectory(glsl)
