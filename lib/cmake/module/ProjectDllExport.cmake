include_guard()

set(__dll_export_file "dll-export.h" CACHE STRING "Name of the file that contains the dll export macro")
set(__dll_export_macro "DLL" CACHE STRING "Name of the dll export macro")

include(GenerateExportHeader)

function(project_dll_export target)
	set(file "${PROJECT_BINARY_DIR}/include/${target}_${__dll_export_file}")

	generate_export_header(${target}
		EXPORT_FILE_NAME ${file}
		EXPORT_MACRO_NAME ${__dll_export_macro}
		INCLUDE_GUARD_NAME ${target}_${__dll_export_macro}_EXPORT_H
	)

	set_property(TARGET ${target} APPEND PROPERTY PUBLIC_HEADER ${file})
	target_project_api(${target})
endfunction()

function(target_project_api)
	foreach(target IN LISTS ARGN)
		target_include_directories(${target}
			PUBLIC
				$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>
		)
	endforeach()
endfunction()
