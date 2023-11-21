#[[
	log_vars(var_names...) will print a readout of all given variables
	and their values. Message modes are supported e.g.: log_vars(... MODE STATUS)

	Parameters
	----------
	<var_names>... :
		Names of variables to print

	MODE <mode> :
		Message mode. Defaults to STATUS

	SPLIT_LISTS :
		Print list values on separate lines
]]
function(log_vars)
	cmake_parse_arguments(PARSE_ARGV 0 args "SPLIT_LISTS" "MODE" "")
	foreach(var IN LISTS args_UNPARSED_ARGUMENTS)
		if("${${var}}" STREQUAL "")
			set(${var} "(empty)")  # value replacement lost outside function scope
		endif()
		if(args_SPLIT_LISTS)
			list(LENGTH ${var} length)
			if(length GREATER 1)
				string(PREPEND ${var} "(list:);")
				string(REPLACE ";" "\n  - " ${var} "${${var}}")
			endif()
		endif()
		set(message "${message} \${${var}} : ${${var}}\n")
	endforeach()
	if(message)
		message(${args_MODE} ${message})
	endif()
endfunction()

# For immediate developer utility, force_log_vars() calls log_vars() in FATAL_ERROR mode.
function(force_log_vars)
	list(APPEND ARGV "MODE" "FATAL_ERROR")
	log_vars(${ARGV})
endfunction()

#[[
	Log all variables in the calling scope.
	Pass a regular expression to FILTER to only log variables matching it.
]]
macro(log_all_vars)
	get_cmake_property(_vars VARIABLES)
	cmake_parse_arguments(_args "" "FILTER" "" ${ARGN})
	set(_notice "All variables")
	if(_args_FILTER)
		set(_notice "${_notice} matching regex '${_args_FILTER}'")
		list(FILTER _vars INCLUDE REGEX "${_args_FILTER}")
	endif()
	if(_vars)
		message(STATUS "${_notice}:")
		log_vars(${_vars} SPLIT_LISTS ${_args_UNPARSED_ARGUMENTS})
	else()
		message(STATUS "${_notice}: (none)")
	endif()
endmacro()

include_guard(GLOBAL)  # Always put this before expect() tests to run them only once

function(test_log_does_not_replace_empty_values)
	set(my_var "")
	log_vars(my_var MODE TRACE)
	expect(my_var STREQUAL "")
endfunction()
test_log_does_not_replace_empty_values()
