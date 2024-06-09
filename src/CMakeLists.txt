set(target "project")

configure_file("version.h.in" "version.h")

add_library(${target}
	project.cxx
	${CMAKE_CURRENT_BINARY_DIR}/version.h
)

add_subdirectory(utility)

target_include_directories(${target}
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
		$<INSTALL_INTERFACE:include>
)
target_link_libraries(${target}
	PUBLIC
		fmt::fmt
)
target_compile_features(${target}
	PUBLIC
		cxx_std_${CMAKE_CXX_STANDARD}
)
target_compile_options(${target}
	PRIVATE
		# https://cmake.org/cmake/help/latest/variable/CMAKE_LANG_COMPILER_ID.html
		$<$<CXX_COMPILER_ID:MSVC>:/W4>
		$<$<CXX_COMPILER_ID:AppleClang,Clang,GNU>:-Wall -Wextra>
)

set(header_files_to_package
	"${CMAKE_CURRENT_BINARY_DIR}/version.h"
	"${CMAKE_CURRENT_LIST_DIR}/project.hxx"
	"${CMAKE_CURRENT_LIST_DIR}/utility/log.hxx"
)

set_target_properties(${target} PROPERTIES
	PUBLIC_HEADER "${header_files_to_package}"
)

project_dll_export(${target})
