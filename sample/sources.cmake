#[[
	This file is included by:
	- sample/CMakeLists.txt
	- sample/targets.cmake

	To each, ${target} refers to a separate target.
]]

set(source_dir "${CMAKE_CURRENT_LIST_DIR}")

target_sources(${target}  # parent scope defines ${target}
	PRIVATE
		${source_dir}/main.cxx

		${source_dir}/cli.cxx
		${source_dir}/cli.hxx
)
target_include_directories(${target}
	PRIVATE
		${CMAKE_CURRENT_LIST_DIR}
)

unset(source_dir)
