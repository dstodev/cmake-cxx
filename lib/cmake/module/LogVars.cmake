include_guard()

#[[
	log_vars(var_names... [MODE <mode>] [RAW_LISTS])

	Print a table of all given variables and their values.

	Parameters
	----------
	MODE <mode> :
		Message mode. Defaults to STATUS

	RAW_LISTS :
		Do not print list values on separate lines
]]
function(log_vars)
	cmake_parse_arguments(PARSE_ARGV 0 args "RAW_LISTS" "MODE" "")
	if(args_MODE MATCHES "^(STATUS|VERBOSE|DEBUG|TRACE)$")
		set(line_prefix "-- ")
	elseif(args_MODE MATCHES "WARNING|ERROR")
		set(indent " ")
	endif()
	foreach(var IN LISTS args_UNPARSED_ARGUMENTS)
		if("${${var}}" STREQUAL "")
			set(${var} "(empty)")  # value replacement lost outside function scope
		endif()
		if(args_RAW_LISTS)
			string(REPLACE ";" "\;" ${var} "${${var}}")
		else()
			list(LENGTH ${var} length)
			if(length GREATER 1)
				string(PREPEND ${var} "(list:);")
				string(REPLACE ";" "\n${indent}${line_prefix} - " ${var} "${${var}}")
			endif()
		endif()
		list(APPEND msg "${indent}\${${var}} : ${${var}}")
	endforeach()
	list(JOIN msg "\n${line_prefix}" msg)
	message(${args_MODE} "${msg}")
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
	cmake_parse_arguments(_args "" "FILTER;MODE" "" ${ARGN})
	set(_notice "All variables")
	if(_args_FILTER)
		set(_notice "${_notice} matching regex '${_args_FILTER}'")
		list(FILTER _vars INCLUDE REGEX "${_args_FILTER}")
	endif()
	if(_vars)
		if(_args_MODE)
			set(_mode_passthrough "MODE;${_args_MODE}")
		endif()
		message(${_args_MODE} "${_notice}:")
		log_vars(${_vars} ${_args_UNPARSED_ARGUMENTS} ${_mode_passthrough})
	else()
		message(${_args_MODE} "${_notice}: (none)")
	endif()
endmacro()

include(Expect)
expect_test_preamble()

function(test_log_does_not_replace_empty_values)
	set(my_var "")
	expect(my_var STREQUAL "")
endfunction()
test_log_does_not_replace_empty_values()

function(test_log_vars_output)
	return()  # comment-out to view output

	set(a "value")
	set(b "a;b;c")

	message("----------------------------------------")
	log_vars(a b a RAW_LISTS)
	message("----------------------------------------")
	log_vars(a b a)
	message("----------------------------------------")
	log_vars(a b a RAW_LISTS MODE STATUS)
	message("----------------------------------------")
	log_vars(a b a MODE STATUS)
	message("----------------------------------------")
	log_vars(a b a RAW_LISTS MODE WARNING)
	message("----------------------------------------")
	log_vars(a b a MODE WARNING)
	message("----------------------------------------")
endfunction()
test_log_vars_output()
