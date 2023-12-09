set(target_name "project")

add_library(${target_name})

add_subdirectory(utility)

target_include_directories(${target_name}
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
		$<INSTALL_INTERFACE:include>
)
target_link_libraries(${target_name}
	PUBLIC
		fmt::fmt
)
target_compile_features(${target_name}
	PUBLIC
		cxx_std_20
)

configure_file("version.hxx.in" "version.hxx")

set(header_files_to_package
	"${CMAKE_CURRENT_BINARY_DIR}/version.hxx"
	"${CMAKE_CURRENT_LIST_DIR}/utility/log.hxx"
)

set_target_properties(${target_name} PROPERTIES
	PUBLIC_HEADER "${header_files_to_package}"
)

project_dll_export(${target_name})
