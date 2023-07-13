target_include_directories(${target_name}
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
)

target_sources(${target_name}
	PRIVATE
		render-specs.cxx
		render-specs.hxx
		scene-visitor.cxx
		scene-visitor.hxx
		texture-cache.cxx
		texture-cache.hxx
		texture.cxx
		texture.hxx
)
