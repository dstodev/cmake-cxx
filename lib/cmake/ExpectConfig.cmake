set(fn_name "expect")

function(${fn_name})
	set(prefix "__${fn_name}_args")
	cmake_parse_arguments(PARSE_ARGV 0 ${prefix} "" "" "")
	set(argv "${${prefix}_UNPARSED_ARGUMENTS}")

	list(PREPEND argv "NOT;(")
	list(APPEND argv ")")

	if(${argv})
		_increment_fails()
		string(JOIN " " expr ${argv})
		get_filename_component(file ${CMAKE_CURRENT_LIST_FILE} NAME)
		set(pretty_message "${fn_name}(${expr}) failed in file ${file}:0")
		message(AUTHOR_WARNING "${pretty_message}")
	endif()
endfunction()

function(_increment_fails)
	math(EXPR fails "${test_assert_fails} + 1")
	_set_test_assert_fails(${fails})
endfunction()

function(_set_test_assert_fails value)
	set(doc "Number of test assertion failures")
	set(test_assert_fails ${value} CACHE INTERNAL "${doc}")
endfunction()

function(error_if_any_${fn_name}_fail)
	if(test_assert_fails GREATER 0)
		message(FATAL_ERROR "${fn_name}() failed ${test_assert_fails} time(s)!")
	endif()
endfunction()

include_guard(GLOBAL)
_set_test_assert_fails(0)
