target_sources(${target_name}
	PRIVATE
		renderer.cxx
		renderer.hxx
		scene-render-visitor.cxx
		scene-render-visitor.hxx
		shader-factory.cxx
		shader-factory.hxx
		shader-program.cxx
		shader-program.hxx
		square.cxx
		square.hxx
		texture.cxx
		texture.hxx
		vao.cxx
		vao.hxx
)
target_include_directories(${target_name}
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
)

find_package(FileToRawStringLiteral CONFIG REQUIRED)
file_to_raw_string_literal("shader/xr-yg-zb.fs" TARGET ${target_name})
file_to_raw_string_literal("shader/xr-yg-zb.vs" TARGET ${target_name})
