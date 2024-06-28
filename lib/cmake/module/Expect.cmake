# CMake is a complicated language. This module is for developers to ensure your
# CMake code is working as you expect it to, including using expect() to write
# "unit tests" for CMake functions.

include_guard()

#[[
	expect(expr...) asserts that expr evaluates TRUE. If expr instead evaluates FALSE, then
	the expect() call "fails", and immediately emits a warning message.
	When configuration completes, failures are counted and expressed by message(FATAL_ERROR).


	Parameters
	----------

	expr :
		Expression to test. Can use the same way as if(), e.g. expect("" IN_LIST my_list)


	Options
	-------

	SAFE :
		If provided and expect() fails, the call will not count toward the number
		of expect() failures, so will not contribute toward emitting message(FATAL_ERROR).
		Still emits a warning message on failure.

	REQUIRED :
		If provided and expect() fails, emits message(FATAL_ERROR) immediately.
		Because it emits FATAL_ERROR immediately, SAFE has no effect.

	MESSAGE "custom-error-message" :
		If provided and expect() fails, emits custom-error-message instead of the
		default warning message.


	Description
	-----------

	expect() is useful to assert that the project is "working as expected", and notify
	developers when it is not:

		expect(CMAKE_BUILD_TYPE MATCHES "Debug|Release")

	To use this module: add the module's directory to CMAKE_MODULE_PATH,
	then include and use:

		cmake_minimum_required(VERSION 3.18)
		list(APPEND CMAKE_MODULE_PATH directory/containing/this/file)
		include(Expect)
		expect(CMAKE_BUILD_TYPE MATCHES "Debug|Release")

	Once the CMake directory which includes this module finishes configuring,
	emits a message conveying the total number of expect() calls, or message(FATAL_ERROR)
	if any expect() call failed. For this reason, developers should include this
	module near the top of the top-level CMakeLists.txt.

	-- expect() output

	If an assertion fails, you will see a message e.g.:

	set(my_var 10)
	expect(my_var EQUAL 11)

	>  CMake Warning (dev) at lib/cmake/module/Expect.cmake:178:EVAL:1 (message):
	>     expect(my_var EQUAL 11) failed!
	>     Search call stack for: (expect)
	>  Call Stack (most recent call first):
	>    lib/cmake/module/Expect.cmake:178 (cmake_language)
	>    CMakeLists.txt:97 (expect)

	Developers are encouraged to search the call stack for (expect), here
	showing the assertion failed in CMakeLists.txt at line 97. This is a language
	limitation, as the calling line number is not available to this module.

	Notice the message shows the expect() expression verbatim, above showing the
	name of the variable (my_var). To log the value of my_var instead, use a normal
	variable expansion:

	set(my_var 10)
	expect(${my_var} EQUAL 11)
	       ^^      ^

	>  expect(10 EQUAL 11) failed!
	>  Search call stack for: (expect)

	-- Using expect() to test CMake code:

	Using a simple idiom, developers may test CMake code:

		include(Expect)
		expect_test_preamble()  # Always call before expect() tests so they run only once

		function(test_example)
			set(my_list "1;2")
			expect(1 IN_LIST my_list)
			expect(2 IN_LIST my_list)
			expect(NOT 3 IN_LIST my_list)
		endfunction()
		test_example()

	Developers should call expect_test_preamble() before expect() tests so the tests
	run only once when modules calling expect() are e.g. included more than once.
	For each file testing with expect(), this preamble must be evaluated before
	running the first test calling expect(), but only one preamble is necessary per file.

	If module developers do not call expect_test_preamble() before expect() tests, then
	printed metrics (number of expect() passes/fails) are invalid: some calls may or may not
	run multiple times, counting as multiple pass/fails.

	When used as suggested, expect() tests run every CMake configure. This means CMake code
	is tested every time the project is configured, asserting the project configures
	as expected.

	-- Disabling expect():

	To disable expect() tests, simply set var DISABLE_EXPECT to TRUE
	before this module is first included.
]]
function(expect)
	_increment_calls()

	math(EXPR max_arg_index "${ARGC} - 1")

	foreach(index RANGE ${max_arg_index})
		# Modify ARGV# in place, escaping backslashes and special characters.

		# Do before calling cmake_parse_arguments() to skip escaping backslashes
		# added by cmake_parse_arguments().

		set(arg "${ARGV${index}}")

		string(REPLACE "\\" "\\\\" arg "${arg}")  # backslashes first

		string(REPLACE "\"" "\\\"" arg "${arg}")
		string(REPLACE "$" "\\$" arg "${arg}")

		string(REPLACE "\t" "\\t" arg "${arg}")
		string(REPLACE "\r" "\\r" arg "${arg}")
		string(REPLACE "\n" "\\n" arg "${arg}")

		set(ARGV${index} "${arg}")
	endforeach()

	set(prefix "__${CMAKE_CURRENT_FUNCTION}")
	# cmake_parse_arguments() adds one escape backslash to semicolons in arguments
	cmake_parse_arguments(PARSE_ARGV 0 ${prefix} "SAFE;REQUIRED" "MESSAGE" "")
	set(argv "${${prefix}_UNPARSED_ARGUMENTS}")
	set(msg "${${prefix}_MESSAGE}")
	set(required ${${prefix}_REQUIRED})
	set(safe ${${prefix}_SAFE})

	set(argv_str "${argv}")

	while(DEFINED argv_str)
		# Look through arguments, wrapping any items with special characters like
		# spaces or semicolons in quotes.
		# Avoid list() operations because they un-escape string elements.
		list_tokenize(argv_str arg)

		if(NOT "${arg}" MATCHES "^[A-Za-z0-9_]+$")
			string(SUBSTRING "${arg}" 0 1 first)

			if(NOT first STREQUAL "\"")
				string(CONCAT arg "\"" "${arg}" "\"")
			endif()
		endif()

		list(APPEND new_argv "${arg}")
	endwhile()

	set(argv "${new_argv}")

	# Manually separate list by replacing non-escaped semicolons with space " "
	# Do not have to worry about escaped backslashes here, because elements
	# with any escaped semicolons are surrounded by quotes.
	string(REGEX REPLACE "([^\\]);" "\\1 " argv "${argv}")
	string(REPLACE "\;" ";" expr "${argv}")  # Un-escape semicolons after separating for if()

	#[[
		Use cmake_language(EVAL CODE) to support wrapping arguments in quotes.
		Cannot use quote literals \"\" because they are interpreted as string
		elements, not CMake syntax.
		Cannot otherwise inject quotes around arguments.
	]]
	set(code "
		if(NOT (${expr}))
			if(NOT ${safe})
				_increment_fails()
			endif()
			set(print TRUE)
		endif()
	")
	cmake_language(EVAL CODE "${code}")

	if(print)
		if(${${prefix}_REQUIRED})
			set(message_mode FATAL_ERROR)
		else()
			set(message_mode AUTHOR_WARNING)
		endif()

		if(msg)
			set(pretty_message "${msg}")
		else()
			string(REPLACE "\\\\" "\\" argv "${argv}")
			set(pretty_message
				" expect(${argv}) failed!\n"
				" Search call stack for: (expect)")
		endif()
		cmake_language(EVAL CODE "${__expect_message_fn}(\${message_mode} \${pretty_message})")
	endif()
endfunction()

function(list_tokenize list_to_consume next_token)
	set(argv_str "${${list_to_consume}}")
	set(find_str "${argv_str}")
	set(pos_offset 0)
	unset(pos)

	# Find first un-escaped semicolon in argv_str, or end of string
	while(NOT pos EQUAL -1)
		string(FIND "${find_str}" ";" pos)
		string(SUBSTRING "${find_str}" 0 ${pos} arg_candidate)

		if(arg_candidate MATCHES "\\\\$")
			# If there is a backslash before the semicolon, determine
			# if the backslash itself is escaped
			string(REGEX MATCH "(\\\\)+$" slashes "${arg_candidate}")
			string(LENGTH "${slashes}" slashes_len)
			math(EXPR result "${slashes_len} % 2")

			if(result EQUAL 0)
				# If there is an even number of backslashes then the backslash
				# is escaped, not the semicolon.
				break()
			endif()

			# Semicolon is escaped, continue searching
			math(EXPR upto_semicolon "${pos} + 1")  # upto and including semicolon
			math(EXPR pos_offset "${pos_offset} + ${upto_semicolon}")
			string(SUBSTRING "${find_str}" ${upto_semicolon} -1 find_str)
		else()
			break()
		endif()
	endwhile()

	if(pos GREATER -1)  # If semicolon found
		math(EXPR pos "${pos} + ${pos_offset}")
	endif()
	string(SUBSTRING "${argv_str}" 0 ${pos} arg)

	if(pos GREATER -1)  # If list has more elements to parse
		# Remove arg from argv_str
		math(EXPR pos "${pos} + 1")
		string(SUBSTRING "${argv_str}" ${pos} -1 argv_str)
		set(${list_to_consume} "${argv_str}" PARENT_SCOPE)
	else()
		unset(${list_to_consume} PARENT_SCOPE)
	endif()

	set(${next_token} "${arg}" PARENT_SCOPE)
endfunction()

function(_increment_calls)
	math(EXPR calls "${__expect_calls} + 1")
	_set_calls(${calls})
endfunction()

function(_set_calls value)
	set(doc "Number of expect() calls")
	set(__expect_calls ${value} CACHE INTERNAL "${doc}")
endfunction()

function(report_expect_calls)
	message(STATUS "expect() performed ${__expect_calls} assertion(s)!")
endfunction()

function(_increment_fails)
	math(EXPR fails "${__expect_fails} + 1")
	_set_fails(${fails})
endfunction()

function(_set_fails value)
	set(doc "Number of times expect() evaluated FALSE")
	set(__expect_fails ${value} CACHE INTERNAL "${doc}")
endfunction()

function(error_if_any_expect_fail)
	if(__expect_fails GREATER 0)
		message(FATAL_ERROR "expect() failed ${__expect_fails} time(s)!")
	endif()
endfunction()

function(_expect_message_nop)
endfunction()

macro(disable_expect_message)
	set(__expect_message_fn _expect_message_nop)
endmacro()

# This module should be included only once, near the top of the top-level CMakeLists.txt.
expect(NOT COMMAND _expect MESSAGE "expect() redefined!" SAFE)

#[[
	Always call expect_test_preamble() before expect() tests to run them only once.
]]
macro(expect_test_preamble)
	include_guard(GLOBAL)
	if(DISABLE_EXPECT)
		return()
	endif()
endmacro()

expect_test_preamble()

_set_fails(0)
_set_calls(0)

set(__expect_message_fn message CACHE INTERNAL "Name of expect() message function")

cmake_language(DEFER CALL report_expect_calls)  # https://cmake.org/cmake/help/latest/command/cmake_language.html#defer
cmake_language(DEFER CALL error_if_any_expect_fail)

###################
#  Test expect()  #
###################

function(test_expect)
	set(check_output FALSE)

	if(check_output)
		# Keyword for intentional failure case
		set(not NOT)
	else()
		# Keyword for normally-negated case
		set(negate NOT)
	endif()

	expect(${not} "" STREQUAL "")
	expect(${not} TRUE)
	expect(${negate} FALSE)
	expect(${not} "a b" STREQUAL "a b")
	expect(${not} "c;d" STREQUAL "c;d")
	expect(${not} "e; f" STREQUAL "e; f")
	expect(${not} "a b;c\;d" STREQUAL "a b;c\;d")
	expect(${not} "a\nb" STREQUAL "a\nb")
	expect(${not} "a\\b" STREQUAL "a\\b")
	expect(${not} "\\" STREQUAL "\\")
	expect(${not} "\\\\" STREQUAL "\\\\")
	expect(${not} "\;" STREQUAL "\;")
	expect(${not} "\\\;" STREQUAL "\\\;")

	expect(${not} "\";\"" STREQUAL "\";\"")
	expect(${negate} "\";\"" STREQUAL ";")

	set(my_list ";;1;2; ; 3;;")  # trailing semicolon puts empty string i.e. "" at end
	list(LENGTH my_list len)
	expect(${not} ${len} EQUAL 8)

	expect(${not} 1 IN_LIST my_list)
	expect(${not} 2 IN_LIST my_list)
	expect(${not} "" IN_LIST my_list)
	expect(${not} " " IN_LIST my_list)
	expect(${not} " 3" IN_LIST my_list)
	expect(${negate} 3 IN_LIST my_list)
	expect(${not} my_list STREQUAL ";;1;2; ; 3;;")
	expect(${not} my_list STREQUAL "${my_list}")

	set(my_list ";\;;\;;")
	list(LENGTH my_list len)
	expect(${not} ${len} EQUAL 4)

	expect(${not} "${my_list}" STREQUAL ";\;;\;;")
	expect(${not} ";" IN_LIST my_list)
	expect(${not} "" IN_LIST my_list)

	set(my_list "; ;  ;   ")
	list(LENGTH my_list len)
	expect(${not} ${len} EQUAL 4)

	expect(${not} "" IN_LIST my_list)
	expect(${not} " " IN_LIST my_list)
	expect(${not} "  " IN_LIST my_list)
	expect(${not} "   " IN_LIST my_list)
	expect(${negate} "    " IN_LIST my_list)
	expect(${not} my_list STREQUAL "; ;  ;   ")
	expect(${negate} "${my_list}" STREQUAL "   ;  ; ;")
endfunction()
test_expect()

function(test_expect_safe)
	disable_expect_message()
	expect(SAFE FALSE)
	expect(FALSE SAFE)
	expect("1" STREQUAL SAFE "2")  # try something really weird
endfunction()
test_expect_safe()

function(test_expect_message)
	disable_expect_message()
	expect(FALSE MESSAGE "Assertion failed!" SAFE)
endfunction()
test_expect_message()

function(test_required)
	expect(TRUE REQUIRED)
	#expect(FALSE MESSAGE "Comment-out this test!" REQUIRED)  # uncomment to check error output
endfunction()
test_required()

function(test_expect_escape_dollar)
	set(a "value")
	expect("\${a}: value" STREQUAL "\${a}: ${a}")
	expect(NOT "\${a}" STREQUAL "${a}")
endfunction()
test_expect_escape_dollar()

##########################
#  Test list_tokenize()  #
##########################

function(test_list_tokenize_simple)
	set(list "a;b;c")

	list_tokenize(list next)
	expect("a" STREQUAL "${next}")
	expect("b;c" STREQUAL "${list}")

	list_tokenize(list next)
	expect("b" STREQUAL "${next}")
	expect("c" STREQUAL "${list}")

	list_tokenize(list next)
	expect("c" STREQUAL "${next}")
	expect(NOT DEFINED list)

	list_tokenize(list next)
	expect("" STREQUAL "${next}")
	expect(NOT DEFINED list)
endfunction()
test_list_tokenize_simple()

function(test_list_tokenize_escaped)
	set(list "a\;b;c\;d;e")

	list_tokenize(list next)
	expect("a\;b" STREQUAL "${next}")
	expect("c\;d;e" STREQUAL "${list}")

	list_tokenize(list next)
	expect("c\;d" STREQUAL "${next}")
	expect("e" STREQUAL "${list}")

	list_tokenize(list next)
	expect("e" STREQUAL "${next}")
	expect(NOT DEFINED list)
endfunction()
test_list_tokenize_escaped()

function(test_list_tokenize_empty)
	set(list "")

	list_tokenize(list next)
	expect("" STREQUAL "${next}")
	expect(NOT DEFINED list)
endfunction()
test_list_tokenize_empty()

function(test_list_tokenize_escaped_separator)
	set(list "\;")

	list_tokenize(list next)
	expect("\;" STREQUAL "${next}")
	expect(NOT DEFINED list)
endfunction()
test_list_tokenize_escaped_separator()

function(test_list_tokenize_escaped_separators)
	set(list "\;;\;")

	list_tokenize(list next)
	expect("\;" STREQUAL "${next}")
	expect("\;" STREQUAL "${list}")

	list_tokenize(list next)
	expect("\;" STREQUAL "${next}")
	expect(NOT DEFINED list)
endfunction()
test_list_tokenize_escaped_separators()

function(test_list_tokenize_escaped_backslash)
	set(list "\\")

	list_tokenize(list next)
	expect("\\" STREQUAL "${next}")
	expect(NOT DEFINED list)
endfunction()
test_list_tokenize_escaped_backslash()

function(test_list_tokenize_empty_string)
	set(list ";")

	list_tokenize(list next)
	expect("" STREQUAL "${next}")
	expect("" STREQUAL "${list}")

	list_tokenize(list next)
	expect("" STREQUAL "${next}")
	expect(NOT DEFINED list)
endfunction()
test_list_tokenize_empty_string()

function(test_list_tokenize_empty_strings)
	set(list ";a;;b;c;")

	list_tokenize(list next)
	expect("" STREQUAL "${next}")
	expect("a;;b;c;" STREQUAL "${list}")

	list_tokenize(list next)
	expect("a" STREQUAL "${next}")
	expect(";b;c;" STREQUAL "${list}")

	list_tokenize(list next)
	expect("" STREQUAL "${next}")
	expect("b;c;" STREQUAL "${list}")

	list_tokenize(list next)
	expect("b" STREQUAL "${next}")
	expect("c;" STREQUAL "${list}")

	list_tokenize(list next)
	expect("c" STREQUAL "${next}")
	expect("" STREQUAL "${list}")

	list_tokenize(list next)
	expect("" STREQUAL "${next}")
	expect(NOT DEFINED list)
endfunction()
test_list_tokenize_empty_strings()

function(test_list_tokenize_escaped_backslash_and_separator)
	set(check_output FALSE)

	if(check_output)
		set(not NOT)
	else()
		set(negate NOT)
	endif()

	set(case_1 "\\\\")    # -> \\
	set(case_2 "\\;")     # -> \;
	set(case_3 "\\\\\;")  # -> \\\;
	set(case_4 "\;")      # -> \;

	set(list "${case_1};${case_2};${case_3};${case_4}")

	list_tokenize(list next)
	expect(${not} "\\\\" STREQUAL "${next}")
	expect(${not} "\\;;\\\\\;;\;" STREQUAL "${list}")

	list_tokenize(list next)
	expect(${not} "\\;" STREQUAL "${next}")
	expect(${not} "\\\\\;;\;" STREQUAL "${list}")

	list_tokenize(list next)
	expect(${not} "\\\\\;" STREQUAL "${next}")
	expect(${not} "\;" STREQUAL "${list}")

	list_tokenize(list next)
	expect(${not} "\;" STREQUAL "${next}")
	expect(${negate} DEFINED list)
endfunction()
test_list_tokenize_escaped_backslash_and_separator()
