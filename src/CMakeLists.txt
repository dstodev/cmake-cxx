set(target_name "project")
set(export_name "project")

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
set_target_properties(${target_name} PROPERTIES
	PUBLIC_HEADER "${header_files_to_package}"
)
target_compile_features(${target_name}
	PUBLIC
		cxx_std_20  # for helpful features like [[nodiscard]] and operator<=>
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

# Package the application
install(TARGETS project eigen SDL2
	EXPORT ${export_name}
)
install(DIRECTORY "${eigen_SOURCE_DIR}/Eigen" DESTINATION include)
install(DIRECTORY "${SDL2_SOURCE_DIR}/include/" DESTINATION include/SDL2)

# Redistribute system runtimes
set(CMAKE_INSTALL_DEBUG_LIBRARIES TRUE)
set(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_SKIP TRUE)
include(InstallRequiredSystemLibraries)

set(release_runtimes ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS})
list(FILTER release_runtimes EXCLUDE REGEX "Debug")

set(debug_runtimes ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS})
list(FILTER debug_runtimes INCLUDE REGEX "Debug")

install(PROGRAMS ${release_runtimes} DESTINATION bin CONFIGURATIONS Release)
install(PROGRAMS ${debug_runtimes} DESTINATION bin CONFIGURATIONS Debug)

log_vars(release_runtimes debug_runtimes SPLIT_LISTS MODE DEBUG)

# Export CMake targets
install(EXPORT ${export_name}
	DESTINATION cmake
	NAMESPACE project::
)
