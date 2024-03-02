# CMake is a complicated language. This module is for developers to ensure your
# CMake code is working as you expect it to, including using expect() to write
# "unit tests" for CMake functions.

include_guard()

#[[
	expect(expr...) asserts that expr evaluates TRUE. If expr instead evaluates FALSE, then
	the expect() call "fails", and a warning message is immediately emitted.

	Parameters
	----------
	expr :
		Expression to test. Can use the same way as if(), e.g. expect("" IN_LIST mylist)

	SAFE :
		If provided and expect() fails, the call will not count toward the number
		of expect() failures, so will not contribute toward emitting message(FATAL_ERROR).
		Still emits a warning message on failure.

	REQUIRED :
		If provided and expect() fails, emits message(FATAL_ERROR) immediately
		instead of a warning.
		Because it emits FATAL_ERROR immediately, SAFE has no effect.

	MESSAGE "custom-error-message" :
		If provided and expect() fails, custom-error-message emits instead of the
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

	If any expect() call fails, emits message(FATAL_ERROR) once the CMake directory
	which first includes this module finishes configuring. For this reason, developers
	should first include this module near the top of the top-level CMakeLists.txt.

	Once the CMake directory which first includes this module finishes configuring,
	emits a message conveying the total number of expect() calls.

	-- expect() output

	If an assertion fails, you will see a message e.g.:

	set(myvar 10)
	expect(myvar EQUAL 11)

	>  CMake Warning (dev) at lib/cmake/module/Expect.cmake:178:EVAL:1 (message):
	>     expect(myvar EQUAL 11) failed!
	>     Search call stack for: (expect)
	>  Call Stack (most recent call first):
	>    lib/cmake/module/Expect.cmake:178 (cmake_language)
	>    CMakeLists.txt:97 (expect)

	Developers are encouraged to search the call stack for (expect), here
	showing the assertion failed in CMakeLists.txt at line 97.

	Notice the message shows the expect() expression verbatim, above showing the
	name of the variable (myvar). To log the value of myvar instead, use a normal
	variable expansion:

	set(myvar 10)
	expect(${myvar} EQUAL 11)

	>  expect(10 EQUAL 11) failed!
	>  Search call stack for: (expect)

	-- Using expect() to unit test CMake code:

	Using a simple idiom, developers may test CMake code:

		include(Expect)
		expect_test_preamble()  # Always call before expect() tests so they run only once

		function(test_example)
			set(mylist "1;2")
			expect(1 IN_LIST mylist)
			expect(2 IN_LIST mylist)
			expect(NOT 3 IN_LIST mylist)
		endfunction()
		test_example()

	Developers should call expect_test_preamble() before expect() tests so the tests
	run only once when modules calling expect() are, for example, included more than
	once. For each file using expect(), this guard should come before the first call
	to expect(), but only once.

	If module developers do not call expect_test_preamble() before expect() tests, then
	printed metrics (number of expect() pass/fail) are invalid; some calls may or may not
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

	set(prefix "__expect_args")
	# cmake_parse_arguments() will escape list semicolons, etc.
	cmake_parse_arguments(PARSE_ARGV 0 ${prefix} "SAFE;REQUIRED" "MESSAGE" "")
	set(argv "${${prefix}_UNPARSED_ARGUMENTS}")
	set(msg "${${prefix}_MESSAGE}")
	set(required ${${prefix}_REQUIRED})
	set(safe ${${prefix}_SAFE})

	set(argv_str "${argv}")

	# Look through arguments, wrapping any items with spaces or semicolons in quotes
	# Avoid list() operations because they un-escape semicolons
	while(NOT done)
		set(find_str "${argv_str}")

		# Find first un-escaped semicolon in argv_str
		while(pos GREATER -1 OR NOT DEFINED pos)
			string(FIND "${find_str}" ";" pos)
			math(EXPR prev "${pos} - 1")

			if(prev LESS 0)
				break()
			endif()

			string(SUBSTRING "${find_str}" ${prev} 1 char)

			if(char STREQUAL "\\")
				math(EXPR upto_semicolon "${pos} + 1")
				string(REPEAT "-" ${upto_semicolon} pad)
				string(SUBSTRING "${find_str}" ${upto_semicolon} -1 pro)
				string(CONCAT find_str "${pad}${pro}")  # Pad to preserve length
			else()
				break()
			endif()
		endwhile()

		string(SUBSTRING "${argv_str}" 0 ${pos} arg)

		if(pos GREATER -1)  # If list has more elements to parse
			# Remove arg from argv_str
			math(EXPR pos "${pos} + 1")
			string(SUBSTRING "${argv_str}" ${pos} -1 argv_str)
		else()
			set(done TRUE)
		endif()

		if("${arg}" MATCHES ";| ")
			string(SUBSTRING "${arg}" 0 1 first)

			if(NOT first STREQUAL "\"")
				string(CONCAT arg "\"" "${arg}" "\"")
			endif()
		endif()

		if(new_argv OR new_argv STREQUAL "")
			string(CONCAT new_argv "${new_argv};${arg}")
		else()
			set(new_argv "${arg}")
		endif()
	endwhile()

	if(new_argv)
		set(argv "${new_argv}")
	endif()

	# Replace empty strings with literal quote pairs
	string(REGEX REPLACE "^;" "\"\";" argv "${argv}")
	string(REGEX REPLACE ";;" ";\"\";" argv "${argv}")
	string(REGEX REPLACE ";$" ";\"\"" argv "${argv}")

	# Manually separate list by replacing non-escaped semicolons with space " "
	# Avoid string(JOIN " " argv ${argv}) because it un-escapes passed-in lists
	string(REGEX REPLACE "([^\\]);" "\\1 " argv "${argv}")
	string(REPLACE "\\;" ";" expr "${argv}")  # Un-escape semicolons after separating for if()

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
			set(pretty_message
				" expect(${argv}) failed!\n"
				" Search call stack for: (expect)")
		endif()
		cmake_language(EVAL CODE "${__expect_message_fn}(\${message_mode} \${pretty_message})")
	endif()
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

function(test_expect)
	expect("" STREQUAL "")
	expect(TRUE)
	expect(NOT FALSE)
	expect("a b" STREQUAL "a b")
	expect("c;d" STREQUAL "c;d")
	expect("e; f" STREQUAL "e; f")
	expect("a b;c\;d" STREQUAL "a b;c\;d")

	set(mylist ";;1;2; ; 3;;")  # trailing semicolon puts empty string i.e. "" at end
	# unset(mylist)  # uncomment to check error output
	expect(1 IN_LIST mylist)
	expect(2 IN_LIST mylist)
	expect("" IN_LIST mylist)
	expect(" " IN_LIST mylist)
	expect(" 3" IN_LIST mylist)
	expect(NOT 3 IN_LIST mylist)
	expect(mylist STREQUAL ";;1;2; ; 3;;")
	expect(mylist STREQUAL "${mylist}")

	set(mylist ";\;;\;;")
	expect(mylist STREQUAL ";\;;\;;")

	expect(NOT "; ;  ;   " STREQUAL "   ;  ; ;")
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
