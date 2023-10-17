file(GLOB_RECURSE shader_files
	"${CMAKE_CURRENT_LIST_DIR}/*.fs"
	"${CMAKE_CURRENT_LIST_DIR}/*.vs"
)
foreach(shader_file IN LISTS shader_files)
	file_to_raw_string_literal(${shader_file} TARGET ${target_name})
endforeach()
