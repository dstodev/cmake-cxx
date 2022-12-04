set(fn_name "expect")

function(${fn_name})
	_increment_calls()

	set(prefix "__${fn_name}_args")
	# cmake_parse_arguments() will escape list semicolons, etc.
	cmake_parse_arguments(PARSE_ARGV 0 ${prefix} "" "" "")
	set(argv "${${prefix}_UNPARSED_ARGUMENTS}")

	string(JOIN " " expr ${${prefix}_UNPARSED_ARGUMENTS})
	get_filename_component(file ${CMAKE_CURRENT_LIST_FILE} NAME)
	set(pretty_message "${fn_name}(${expr}) failed in file ${file}:0")

	# Uses cmake_language(EVAL CODE) https://cmake.org/cmake/help/latest/command/cmake_language.html#evaluating-code
	# to handle empty strings in ${argv}
	#   e.g. argv: ;;IN_LIST;mylist; fails because it expands to if(NOT(IN_LIST mylist)
	#        so replace empty strings with quotes so it expands instead to if(NOT("" IN_LIST mylist)
	# This cannot be done using if("${argv}") in the current scope, as it would instead expand
	# to a literal string if("NOT;(;;IN_LIST;args_UNPARSED_ARGUMENTS;)") which evaluates FALSE.
	string(REGEX REPLACE "^;|;;|;$" " \"\" " argv "${argv}")
	cmake_language(EVAL CODE "
		if(NOT (${argv}))
			_increment_fails()
			message(AUTHOR_WARNING \"${pretty_message}\")
		endif()
	")
endfunction()

function(_increment_calls)
	math(EXPR calls "${${fn_name}_calls} + 1")
	_set_calls(${calls})
endfunction()

function(_set_calls value)
	set(doc "Number of ${fn_name}() calls")
	set(${fn_name}_calls ${value} CACHE INTERNAL "${doc}")
endfunction()

function(report_${fn_name}_calls)
	message("${fn_name}() performed ${${fn_name}_calls} assertion(s)!")
endfunction()

function(_increment_fails)
	math(EXPR fails "${${fn_name}_fails} + 1")
	_set_fails(${fails})
endfunction()

function(_set_fails value)
	set(doc "Number of times ${fn_name}() evaluated FALSE")
	set(${fn_name}_fails ${value} CACHE INTERNAL "${doc}")
endfunction()

function(error_if_any_${fn_name}_fail)
	if(${fn_name}_fails GREATER 0)
		message(FATAL_ERROR "${fn_name}() failed ${${fn_name}_fails} time(s)!")
	endif()
endfunction()

include_guard(GLOBAL)  # Always put this before expect() tests to run only once

_set_fails(0)
_set_calls(0)

cmake_language(DEFER CALL report_${fn_name}_calls)  # https://cmake.org/cmake/help/latest/command/cmake_language.html#defer
cmake_language(DEFER CALL error_if_any_${fn_name}_fail)

function(test_expect)
	set(mylist "1;2")

	expect("" STREQUAL "")
	expect(TRUE)
	expect(NOT FALSE)
	expect(1 IN_LIST mylist)
	expect(2 IN_LIST mylist)
	expect(NOT 3 IN_LIST mylist)
endfunction()
test_expect()
