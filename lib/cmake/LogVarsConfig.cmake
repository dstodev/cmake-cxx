#[[
	log_vars(var_names...) will print a readout of all given variables
	and their values. Message modes are supported like MODE FATAL_ERROR.

	Parameters
	----------
	var_names...
		Names of variables to print

	MODE mode
		Message mode; defaults to STATUS
]]
function(log_vars)
	cmake_parse_arguments(PARSE_ARGV 0 args "" "MODE" "")
	foreach(var IN LISTS args_UNPARSED_ARGUMENTS)
		if("${${var}}" STREQUAL "")
			set(${var} "(empty)")  # value replacement lost outside function scope
		endif()
		set(message "${message} <> ${var} : ${${var}}\n")
	endforeach()
	message(${args_MODE} "${message}")
endfunction()

# For immediate developer utility, force_log_vars() calls log_vars() in FATAL_ERROR mode.
function(force_log_vars)
	list(APPEND ARGV "MODE" "FATAL_ERROR")
	log_vars(${ARGV})
endfunction()

include_guard(GLOBAL)  # Always put this before expect() tests to run them only once

function(test_log_does_not_replace_empty_values)
	set(my_var "")
	log_vars(my_var MODE TRACE)
	expect(my_var STREQUAL "")
endfunction()
test_log_does_not_replace_empty_values()
