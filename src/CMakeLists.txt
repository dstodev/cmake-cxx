set(target_name "shear")

set(header_files_to_package
	shear.hxx
)

set(dll_export_macro SHEAR_API)

#[[
	Configure the library
]]
add_library(${target_name}
	shear.cxx
	shear.hxx
)
set_target_properties(${target_name}
	PROPERTIES
		PUBLIC_HEADER "${header_files_to_package}"
)
target_include_directories(${target_name}
	PUBLIC  # Could be INTERFACE if only using relative includes e.g. #include "shear.hxx"
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
		$<INSTALL_INTERFACE:include>
)
target_compile_features(${target_name}
	PUBLIC
		cxx_std_11
)

find_package(Eigen3 REQUIRED)

target_link_libraries(${target_name}
	PUBLIC
		Eigen3::Eigen
)

#[[
	The header this part describes will be generated to the current binary
	directory as ${CMAKE_CURRENT_BINARY_DIR}/${target_name}_export.h
]]
include(GenerateExportHeader)
generate_export_header(${target_name}
	EXPORT_MACRO_NAME ${dll_export_macro}
)
target_include_directories(${target_name}
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
		$<INSTALL_INTERFACE:include>
)
set_property(TARGET ${target_name}
	APPEND PROPERTY PUBLIC_HEADER
		"${CMAKE_CURRENT_BINARY_DIR}/${target_name}_export.h"
)

#[[
	Package the library
]]
install(TARGETS ${target_name}
	EXPORT ${target_name}
)
install(EXPORT ${target_name}
	DESTINATION cmake
	NAMESPACE shear::
)
