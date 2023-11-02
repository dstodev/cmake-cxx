# https://github.com/nigels-com/glew

if(WIN32)
	# https://github.com/nigels-com/glew/releases
	set(url "https://github.com/nigels-com/glew/releases/download/glew-2.2.0/glew-2.2.0-win32.zip")
	# set(url "https://github.com/nigels-com/glew/releases/download/glew-2.2.0/glew-2.2.0.tgz")

	CPMAddPackage(NAME glew
		URL ${url}
		DOWNLOAD_ONLY TRUE
	)
	set(target_name GLEW::GLEW)

	add_library(${target_name} IMPORTED ${lib_type})
	if(BUILD_SHARED_LIBS)
		set_target_properties(${target_name} PROPERTIES
			IMPORTED_LOCATION "${glew_SOURCE_DIR}/bin/Release/x64/glew32.dll"
			IMPORTED_IMPLIB "${glew_SOURCE_DIR}/lib/Release/x64/glew32.lib"
		)
		set(runtime_output_directory "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
		configure_file("${glew_SOURCE_DIR}/bin/Release/x64/glew32.dll" "${runtime_output_directory}/glew32.dll" COPYONLY)
	else()
		set_target_properties(${target_name} PROPERTIES
			IMPORTED_LOCATION "${glew_SOURCE_DIR}/lib/Release/x64/glew32s.lib"
		)
	endif()
	target_include_directories(${target_name} INTERFACE
		$<BUILD_INTERFACE:${glew_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
	)
elseif(APPLE)
	find_package(GLEW)  # REQUIRED keyword is not respected by FindGLEW.cmake
	if(NOT GLEW_FOUND)
		message(FATAL_ERROR " Could not find GLEW! Please install it e.g. brew install glew")
	endif()
endif()
