target_sources(${target}  # parent scope defines ${target}
	PRIVATE
		log.cxx
		log.hxx
)
target_include_directories(${target}
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
)
target_compile_definitions(${target}
	PUBLIC
		$<$<CONFIG:Debug>:ENABLE_LOGGING>
)
