target_include_directories(${target_name}
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
)

target_sources(${target_name}
	PRIVATE
		reader.cxx
		reader.hxx
		state.cxx
		state.hxx
)
