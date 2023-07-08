target_include_directories(${target_name}
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
)

target_sources(${target_name}
	PRIVATE
		player.cxx
		textures.hxx
)
