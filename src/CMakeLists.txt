set(target_name "project")

set(dll_export_file "project-api.h")
set(dll_export_macro "PROJECT_API")

# Configure the library
add_library(${target_name}
	application-impl.cxx
	application-impl.hxx
	application-state.hxx
	application.cxx
	application.hxx
	game.cxx
	game.hxx
	grid_t.hxx
	log.hxx
	placeholder_t.hxx
	player.cxx
	player.hxx
	point_t.cxx
	point_t.hxx
	render-specs.cxx
	render-specs.hxx
	shear.cxx
	shear.hxx
	textures/player.cxx
	textures/textures.hxx
)
target_include_directories(${target_name}
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
		$<INSTALL_INTERFACE:include>
)
target_link_libraries(${target_name}
	PUBLIC
		Eigen3::Eigen
)
set(header_files_to_package
	application.hxx
	log.hxx
)
list(TRANSFORM header_files_to_package PREPEND "${CMAKE_CURRENT_LIST_DIR}/")
set_target_properties(${target_name} PROPERTIES
	PUBLIC_HEADER "${header_files_to_package}"
)
target_compile_features(${target_name}
	PUBLIC
		cxx_std_20  # for helpful features like [[nodiscard]] and operator<=>
)
target_compile_definitions(${target_name}
	PRIVATE
		$<$<CONFIG:Debug>:ENABLE_LOGGING>
)

#[[
	This call to target_precompile_headers() affects development builds
	(i.e. not sources in an install() context) by prepending
	'#include <placeholder.hxx>' to all sources, exposing the
	placeholder_t type for developer utility.

	This behavior applies in a BUILD_INTERFACE context and not in a
	INSTALL_INTERFACE context, meaning that:

	The project::placeholder_t type is usable on-the-fly while developing,
	as the header is implicitly provided. However, the header is not supplied
	to packages, so packages still using this type will fail to compile.

	This signals that the package should not release; the placeholder type
	should first be replaced with a real type. Therefore, the 'placeholder'
	name and 'type not found' error should make sense in context.
]]
target_precompile_headers(${target_name}
	PUBLIC
		$<BUILD_INTERFACE:placeholder_t.hxx>
)

# Link dependencies
target_link_libraries(${target_name}
	PRIVATE
		Eigen3::Eigen
		SDL2::SDL2
)

# Generate header for DLL export macro
include(GenerateExportHeader)
generate_export_header(${target_name}
	EXPORT_FILE_NAME ${dll_export_file}
	EXPORT_MACRO_NAME ${dll_export_macro}
)
target_include_directories(${target_name}
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
		$<INSTALL_INTERFACE:include>
)
set_property(TARGET ${target_name}
	APPEND PROPERTY PUBLIC_HEADER "${CMAKE_CURRENT_BINARY_DIR}/${dll_export_file}"
)
