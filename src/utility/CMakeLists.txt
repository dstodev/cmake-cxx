target_sources(${target_name}  # parent scope defines ${target_name}
	PRIVATE
		log.cxx
		log.hxx
		deduplicate.hxx
)
target_include_directories(${target_name}
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
)
target_compile_definitions(${target_name}
	PUBLIC
		$<$<CONFIG:Debug>:ENABLE_LOGGING>
)
