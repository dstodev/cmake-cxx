set(fn_name "expect")

function(${fn_name})
	_increment_calls()

	set(prefix "__${fn_name}_args")
	# cmake_parse_arguments() will escape list semicolons, etc.
	cmake_parse_arguments(PARSE_ARGV 0 ${prefix} "" "" "")
	set(argv "${${prefix}_UNPARSED_ARGUMENTS}")
	list(GET argv 0 first_token)

	if(first_token STREQUAL "")
		list(LENGTH argv len)

		if(len GREATER 1)
			list(SUBLIST argv 1 -1 rest_of_tokens)
		endif()

		if(NOT "" ${rest_of_tokens})
			set(fail TRUE)
		endif()
	else()
		if(NOT (${argv}))
			set(fail TRUE)
		endif()
	endif()

	if(fail)
		_increment_fails()
		string(JOIN " " expr ${${prefix}_UNPARSED_ARGUMENTS})
		get_filename_component(file ${CMAKE_CURRENT_LIST_FILE} NAME)
		set(pretty_message "${fn_name}(${expr}) failed in file ${file}:0")
		message(AUTHOR_WARNING "${pretty_message}")
	endif()
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

include_guard(GLOBAL)
_set_fails(0)
_set_calls(0)

cmake_language(DEFER CALL report_${fn_name}_calls)  # https://cmake.org/cmake/help/latest/command/cmake_language.html#defer
cmake_language(DEFER CALL error_if_any_${fn_name}_fail)
