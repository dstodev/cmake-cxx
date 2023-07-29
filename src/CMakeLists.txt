set(target_name "project")

add_library(${target_name})

add_subdirectory(application)
add_subdirectory(input)
add_subdirectory(render)
add_subdirectory(scene)
add_subdirectory(simulation)
add_subdirectory(utility)

target_include_directories(${target_name}
	PUBLIC
		$<INSTALL_INTERFACE:include>
)
target_link_libraries(${target_name}
	PUBLIC
		Eigen3::Eigen
		fmt::fmt

	PRIVATE
		SDL2::SDL2
)
target_compile_features(${target_name}
	PUBLIC
		cxx_std_20
)

set(header_files_to_package
	"${CMAKE_CURRENT_LIST_DIR}/application/application.hxx"
	"${CMAKE_CURRENT_LIST_DIR}/utility/log.hxx"
)

set_target_properties(${target_name} PROPERTIES
	PUBLIC_HEADER "${header_files_to_package}"
)

project_dll_export(${target_name})
