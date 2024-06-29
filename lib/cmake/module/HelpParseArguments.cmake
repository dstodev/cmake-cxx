include_guard()

#[[
	Parses function arguments past the last required function argument of the
	function calling this macro.

	Usage:
		help_parse_arguments(prefix options one_value_keywords multi_value_keywords)

	Helps parse arguments by abstracting technical details of CMake's builtin (>= CMake 3.5)
	cmake_parse_arguments(). Automatically uses the more intuitive but harder to implement
	PARSE_ARGV variant of cmake_parse_arguments() to enable passing e.g. lists.


	Parameters
	----------

	prefix :
		String prefixed to each parsed argument variable delimited with '_'
		e.g. prefix="args" creates variables like "args_OPTION"

	options :
		List of option flags to parse for, e.g. "DO_STUFF;USE_THING"
		For more information, view docs for cmake_parse_arguments(<options>)

	one_value_keywords :
		List of keywords to parse for which expect a single value, e.g. "TARGET;LIB_TYPE"
		Supports stringified lists as the single value.
		For more information, view docs for cmake_parse_arguments(<one_value_keywords>)

	multi_value_keywords :
		List of keywords to parse for which expect multiple values, e.g. "EXTRA_CXX_FLAGS"
		For more information, view docs for cmake_parse_arguments(<multi_value_keywords>)


	Example
	-------

	include(HelpParseArguments)

	function(my_function target)
		help_parse_arguments(args "PRINT_VALUES" "VALUE1;VALUE2" "OTHER_VALUES")

		(args_PRINT_VALUES)
			message("Value 1: ${args_VALUE1}")
			message("Value 2: ${args_VALUE2}")
			foreach (val IN LISTS args_OTHER_VALUES)
				message("Other value: ${val}")
			endforeach ()
		endif()
	endfunction()

	my_function(some_target PRINT_VALUES VALUE1 a b VALUE2 "c;d" OTHER_VALUES e f g)

	Output:
		[cmake] Value 1: a
		[cmake] Value 2: c;d
		[cmake] Other value: e
		[cmake] Other value: f
		[cmake] Other value: g

	References
	----------
	- https://cmake.org/cmake/help/latest/command/cmake_parse_arguments.html
]]
macro(help_parse_arguments prefix options one_value_keywords multi_value_keywords)
	# Variables are prefixed with _ to avoid name collisions in parent scope

	list(LENGTH ARGV _num_argv)  # Total number of arguments passed to the calling function
	list(LENGTH ARGN _num_argn)  # Number of arguments past the last expected parameter
	math(EXPR _expected_args_offset "${_num_argv} - ${_num_argn}")  # Calculated number of positional args

	# Skip past the expected args.
	cmake_parse_arguments(PARSE_ARGV "${_expected_args_offset}" "${prefix}"
		"${options}"
		"${one_value_keywords}"
		"${multi_value_keywords}"
	)

	unset(_num_argv)
	unset(_num_argn)
	unset(_expected_args_offset)

	############################################################################################

	#[[
		# Learned while implementing this macro:
		# To get one of the calling function's special variables e.g. ARGC:

		set(_caller_argc_hack "ARGC")
		set(_caller_argc "${${_caller_argc_hack}}")
		message("Macro's ARGC:  ${ARGC}")
		message("Caller's ARGC: ${_caller_argc}")

		# This works because in a macro, ${ARGC} will be text-replaced (similar to the C/C++
		# preprocessor), but ${${variable_expanding_to_ARGC}} will not be text-replaced, so
		# the resulting ${ARGC} variable is evaluated within the context of the calling function
		# instead of this macro.
	]]
endmacro()

#[[
	Print all variables with the given prefix.

	Useful for printing all variables in a "namespace" i.e. with a common prefix, like from
	help_parse_arguments(prefix ...)
]]
function(help_print_parsed_arguments _prefix)
	# Get all variables first to avoid inclusion of variables used in this function.
	# Variables are prefixed with _ to reduce risk that variables to print have the same name
	# as variables used in this function. Function parameters are also considered variables.
	get_cmake_property(_all_variables VARIABLES)

	help_parse_arguments(_params "" "VERBOSITY" "")

	# Match all variables with prefix
	string(REGEX MATCHALL "(^|;)${_prefix}_[A-Za-z0-9_]*" _prefixed_variables "${_all_variables}")

	# Remove leading & duplicate semicolons
	string(REGEX REPLACE "(^|;);" "\\1" _prefixed_variables "${_prefixed_variables}")

	foreach(_var IN LISTS _prefixed_variables)
		message(${_params_VERBOSITY} "${_var}: ${${_var}}")
	endforeach()
endfunction()

include(Expect)
expect_test_preamble()

function(test_parse_options a b c)
	help_parse_arguments(args "FLAG1;FLAG2" "" "")
	expect(args_FLAG1)
	expect(NOT args_FLAG2)
	expect(1 IN_LIST args_UNPARSED_ARGUMENTS)
	expect(2 IN_LIST args_UNPARSED_ARGUMENTS)
	expect(NOT 3 IN_LIST args_UNPARSED_ARGUMENTS)
endfunction()
test_parse_options(a b c 1 2 FLAG1)

function(test_parse_one_value_keywords a b c)
	help_parse_arguments(args "FLAG1;FLAG2" "OPTION1;OPTION2" "")
	expect(NOT args_FLAG1)
	expect(NOT args_FLAG2)
	expect(d STREQUAL "${args_OPTION1}")
	expect(NOT args_OPTION2)
endfunction()
test_parse_one_value_keywords(a b c OPTION1 d)

function(test_parse_multi_value_keywords a b c)
	help_parse_arguments(args "" "OPTION1;OPTION2" "MULTIS1;MULTIS2")
	expect(NOT args_OPTION1)
	expect(NOT args_OPTION2)
	expect("d;e" STREQUAL "${args_MULTIS1}")
	expect(NOT args_MULTIS2)
endfunction()
test_parse_multi_value_keywords(a b c MULTIS1 d e)

function(test_parse_argv)
	help_parse_arguments(args "" "" "")
	expect("" IN_LIST args_UNPARSED_ARGUMENTS)
	expect("1;2" IN_LIST args_UNPARSED_ARGUMENTS)
	expect("3" IN_LIST args_UNPARSED_ARGUMENTS)
	expect("4" IN_LIST args_UNPARSED_ARGUMENTS)
endfunction()
test_parse_argv("" "1;2" 3 4)
