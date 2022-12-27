set(target_name "project")

set(header_files_to_package
	grid.hxx
	shear.hxx
)

set(dll_export_file "${target_name}-api.h")
set(dll_export_macro PROJECT_API)

# Configure the library
add_library(${target_name}
	grid.hxx
	placeholder.hxx
	shear.cxx
	shear.hxx
	utilities.cxx
	utilities.hxx
)
set_target_properties(${target_name}
	PROPERTIES
		PUBLIC_HEADER "${header_files_to_package}"
)
target_include_directories(${target_name}
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
		$<INSTALL_INTERFACE:include>
)
target_compile_features(${target_name}
	PUBLIC
		cxx_std_20  # for helpful features like [[nodiscard]] and operator<=>
)
target_compile_definitions(${target_name}
	PUBLIC
		SUPPORT_EIGEN
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
		$<BUILD_INTERFACE:placeholder.hxx>
)

# Link dependencies
find_package(Eigen3 REQUIRED)
target_link_libraries(${target_name}
	PUBLIC
		Eigen3::Eigen
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
	APPEND PROPERTY PUBLIC_HEADER
		"${CMAKE_CURRENT_BINARY_DIR}/${dll_export_file}"
)

# Package the library
install(TARGETS ${target_name}
	EXPORT ${target_name}
)
install(EXPORT ${target_name}
	DESTINATION cmake
	NAMESPACE project::
)

# Configure sample executable
set(target_name "main")
add_executable(${target_name}
	main.cxx
)
target_link_libraries(${target_name}
	PRIVATE
		project
)
