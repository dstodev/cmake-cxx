include_guard(DIRECTORY)

set(__default_build_mode "Release" CACHE STRING "Default single-config-generator build mode")

function(setup_build_mode)
	get_cmake_property(is_multi_config GENERATOR_IS_MULTI_CONFIG)  # https://cmake.org/cmake/help/latest/prop_gbl/GENERATOR_IS_MULTI_CONFIG.html

	if (is_multi_config)
		_multi_config()
	else()
		_single_config()
	endif()
endfunction()

function(_multi_config)
	# https://cmake.org/cmake/help/latest/variable/CMAKE_CONFIGURATION_TYPES.html#variable:CMAKE_CONFIGURATION_TYPES
	# Nothing to do; configure for all modes!
endfunction()

function(_single_config)
	if (NOT CMAKE_BUILD_TYPE)  # if not set or if empty string
		message(WARNING "CMAKE_BUILD_TYPE unset or empty. Forcing CMAKE_BUILD_TYPE to: ${__default_build_mode}")
		# Variable may already exist in cache (as e.g. empty string), so FORCE
		set(CMAKE_BUILD_TYPE "${__default_build_mode}" CACHE STRING "Build mode" FORCE)
	endif()
endfunction()
