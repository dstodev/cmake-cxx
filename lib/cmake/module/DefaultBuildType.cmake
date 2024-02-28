include_guard()

include(StringCapitalize)

#[[
	If using a single-config generator, asserts CMAKE_BUILD_TYPE exists.
	Call default_build_type() before project().
]]
function(default_build_type type)
	get_cmake_property(is_multi_config GENERATOR_IS_MULTI_CONFIG)  # https://cmake.org/cmake/help/latest/prop_gbl/GENERATOR_IS_MULTI_CONFIG.html

	if(is_multi_config)
		_multi_config()
	else()
		_single_config(${type})
	endif()
endfunction()

function(_multi_config)
	# https://cmake.org/cmake/help/latest/variable/CMAKE_CONFIGURATION_TYPES.html
	# Nothing to do; configure for all modes!
endfunction()

function(_single_config type)
	if(NOT CMAKE_BUILD_TYPE)  # if not set or if empty string
		string_capitalize(${type} type)
		message(WARNING "CMAKE_BUILD_TYPE unset or empty. Forcing CMAKE_BUILD_TYPE to: ${type}")
		# Variable may already exist in cache (as e.g. empty string), so FORCE
		set(CMAKE_BUILD_TYPE "${type}" CACHE STRING "Build type" FORCE)
	endif()
endfunction()
