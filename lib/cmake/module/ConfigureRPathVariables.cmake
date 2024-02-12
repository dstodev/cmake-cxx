include_guard()

function(configure_rpath_variables)
	if(UNIX)
		if(APPLE)
			set(prefix "@loader_path")
		else()
			set(prefix "$ORIGIN")
		endif()
		set(CMAKE_INSTALL_RPATH "${prefix}/../lib" PARENT_SCOPE)
		set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE PARENT_SCOPE)
	endif()
endfunction()
