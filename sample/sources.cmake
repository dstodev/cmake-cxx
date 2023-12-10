#[[
	This file is included by:
	- sample/CMakeLists.txt
	- sample/targets.cmake
]]

set(source_dir "${CMAKE_CURRENT_LIST_DIR}")

target_sources(${target_name}
	PRIVATE
		${source_dir}/main.cxx

		${source_dir}/cli.cxx
		${source_dir}/cli.hxx
)
target_include_directories(${target_name}
	PRIVATE
		${CMAKE_CURRENT_LIST_DIR}
)
