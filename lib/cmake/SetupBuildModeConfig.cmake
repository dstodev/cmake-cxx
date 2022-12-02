find_package(StringCapitalize CONFIG REQUIRED)

function(setup_build_mode default_mode)
	if (NOT default_mode)
		if ("${default_mode}" STREQUAL "")
			set(hint " (empty string)")
		endif()
		message(FATAL_ERROR "Invalid value for default_mode must evaluate TRUE: \"${default_mode}\"${hint}")
	endif()

	get_cmake_property(is_multi_config GENERATOR_IS_MULTI_CONFIG)  # https://cmake.org/cmake/help/latest/prop_gbl/GENERATOR_IS_MULTI_CONFIG.html
	if (is_multi_config)
		_multi_config()
	else()
		string_capitalize(${default_mode} default_mode)
		_single_config("${default_mode}")
	endif()
endfunction()

function(_multi_config)
	# https://cmake.org/cmake/help/latest/variable/CMAKE_CONFIGURATION_TYPES.html#variable:CMAKE_CONFIGURATION_TYPES
	# TODO
endfunction()

function(_single_config default_mode)
	if (NOT CMAKE_BUILD_TYPE)  # if not set or if empty string
		message(WARNING "CMAKE_BUILD_TYPE unset or evaluated FALSE. Forcing CMAKE_BUILD_TYPE to: ${default_mode}")
		# Variable may already exist in cache (as e.g. empty string), so FORCE
		set(CMAKE_BUILD_TYPE "${default_mode}" CACHE STRING "Build mode" FORCE)
	endif()
endfunction()
