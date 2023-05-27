#[[
	expect(expr) asserts that expr evaluates TRUE. If expr instead evaluates FALSE, then
	the expect() call "fails", and a warning message is immediately emitted.

	Parameters
	----------
	expr
		Expression to test. Can use the same way as if(), e.g. expect("" IN_LIST mylist)

	SAFE
		If provided and expect() fails, the call will not count toward the number
		of expect() failures, so will not contribute toward emitting message(FATAL_ERROR).
		Still emits a warning message on failure.

	REQUIRED
		If provided and expect() fails, emits message(FATAL_ERROR) immediately
		instead of a warning.
		Because it emits FATAL_ERROR immediately, SAFE has no effect.

	MESSAGE "custom-error-message"
		If provided and expect() fails, custom-error-message emits instead of the
		default warning message.

	Description
	-----------
	expect() is useful to assert that the project is "working as expected", and notify
	developers when it is not:

		expect(CMAKE_BUILD_TYPE MATCHES "Debug|Release")

	To use this module, include the module as early as possible (preferably
	near the top of the top-level CMakeLists.txt):

		cmake_minimum_required(VERSION 3.18)
		list(APPEND CMAKE_PREFIX_PATH directory/containing/this/file)
		find_package(Expect CONFIG REQUIRED)

	If any expect() call fails, emits message(FATAL_ERROR) once the CMake directory
	which first includes this module finishes configuring. For this reason, developers
	should include this module only once near the top of the top-level CMakeLists.txt.
	This way, the message emits when the project finishes configuring, conveying the
	total number of expect() failures. Additionally, this allows subsequent modules
	to assume that expect() exists, almost as if it were a built-in command.

	Once the CMake directory which first includes this module finishes configuring,
	emits a message conveying the total number of expect() calls.

	Developers should take care to avoid duplicating expect() calls by e.g. including
	them multiple times with find_package() or include(). If calls are duplicated,
	printed metrics will contain duplicates.

	-- Using expect() to unit test CMake code:

	Using a simple idiom, developers may test CMake code by defining a function
	to introduce scope & purpose, then setting up the environment and calling expect():

		include_guard(GLOBAL)  # Always put this before expect() tests to run them only once

		function(test_example)
			set(mylist "1;2")
			expect(1 IN_LIST mylist)
			expect(2 IN_LIST mylist)
			expect(NOT 3 IN_LIST mylist)
		endfunction()
		test_example()

	Developers should place include_guard(GLOBAL) before expect() tests so the tests
	run only once when modules calling expect() are, for example, included more than
	once. For each file using expect(), this guard should come before the first call
	to expect(), but only once.

	If module developers fail to use include_guard(GLOBAL) before expect() tests, then
	printed metrics (number of expect() pass/fail) are invalidated, as some calls will
	likely run multiple times, counting as multiple pass/fails.

	When used as suggested, expect() tests run every CMake configure. This means CMake code
	is tested every time the project is configured, asserting that the project configures
	in a good state.

	-- Disabling expect():

	Because modules may assume that expect() exists "as a built-in command", expect()
	is not easily removable, but is easily replaceable. To disable expect(), simply
	do not include the Expect module, and instead define a simple replacement function
	that takes any amount of parameters but has no behavior:

		function(expect)
		endfunction()
]]
function(expect)
	_increment_calls()

	set(prefix "__expect_args")
	# cmake_parse_arguments() will escape list semicolons, etc.
	cmake_parse_arguments(PARSE_ARGV 0 ${prefix} "SAFE;REQUIRED" "MESSAGE" "")
	set(argv "${${prefix}_UNPARSED_ARGUMENTS}")
	set(msg ${${prefix}_MESSAGE})
	set(required ${${prefix}_REQUIRED})
	set(safe ${${prefix}_SAFE})

	# Replace empty strings with literal quote pairs
	string(REGEX REPLACE "^;" "\"\";" argv "${argv}")
	string(REGEX REPLACE ";;" ";\"\";" argv "${argv}")
	string(REGEX REPLACE ";$" ";\"\"" argv "${argv}")

	# Manually separate list by replacing non-escaped semicolons with space " "
	# Avoid string(JOIN " " argv ${argv}) because it un-escapes passed-in lists
	string(REGEX REPLACE "([^\\]);" "\\1 " argv "${argv}")

	#[[
		If called like expect("" IN_LIST mylist), then "${argv}" is ";IN_LIST;mylist"
		Evaluating argv like this is invalid because it becomes if(NOT (IN_LIST mylist))
		So, replace empty strings with literal quotes to become if(NOT ("" IN_LIST mylist))
		when evaluated by cmake_language(EVAL CODE)  https://cmake.org/cmake/help/latest/command/cmake_language.html#evaluating-code

		Cannot use if(NOT ("${argv}")) instead, as it becomes if(NOT (";IN_LIST;mylist"))
		which evaluates like if(NOT (TRUE)).
	]]
	set(code "
		if(NOT (${argv}))
			if(NOT ${safe})
				_increment_fails()
			endif()
			set(print TRUE)
		endif()
	")
	cmake_language(EVAL CODE "${code}")
	if (print)
		if (${${prefix}_REQUIRED})
			set(message_mode FATAL_ERROR)
		else()
			set(message_mode AUTHOR_WARNING)
		endif()

		if (msg)
			set(pretty_message "${msg}")
		else()
			string(REPLACE "\"" "\\\"" expr "${argv}")  # double-escape quotes for message
			set(pretty_message
				" expect(${expr}) failed!\n"
				" Search call stack for: (expect)")
		endif()
		message(${message_mode} ${pretty_message})
	endif()
endfunction()

function(_increment_calls)
	math(EXPR calls "${expect_calls} + 1")
	_set_calls(${calls})
endfunction()

function(_set_calls value)
	set(doc "Number of expect() calls")
	set(expect_calls ${value} CACHE INTERNAL "${doc}")
endfunction()

function(report_expect_calls)
	message(STATUS "expect() performed ${expect_calls} assertion(s)!")
endfunction()

function(_increment_fails)
	math(EXPR fails "${expect_fails} + 1")
	_set_fails(${fails})
endfunction()

function(_set_fails value)
	set(doc "Number of times expect() evaluated FALSE")
	set(expect_fails ${value} CACHE INTERNAL "${doc}")
endfunction()

function(error_if_any_expect_fail)
	if(expect_fails GREATER 0)
		message(FATAL_ERROR "expect() failed ${expect_fails} time(s)!")
	endif()
endfunction()

include_guard(GLOBAL)  # Always put this before expect() tests to run them only once

_set_fails(0)
_set_calls(0)

cmake_language(DEFER CALL report_expect_calls)  # https://cmake.org/cmake/help/latest/command/cmake_language.html#defer
cmake_language(DEFER CALL error_if_any_expect_fail)

function(test_expect)
	expect("" STREQUAL "")
	expect(TRUE)
	expect(NOT FALSE)

	set(mylist "1;2;")  # trailing semicolon puts empty string i.e. "" at end
	# unset(mylist)  # uncomment to check error output
	expect(1 IN_LIST mylist)
	expect(2 IN_LIST mylist)
	expect(NOT 3 IN_LIST mylist)
	expect("" IN_LIST mylist)
endfunction()
test_expect()

function(test_expect_safe)
	set(CMAKE_MESSAGE_LOG_LEVEL ERROR)  # comment-out to check error output for this function
	expect(SAFE FALSE)
	expect(FALSE SAFE)
	expect("1" STREQUAL SAFE "2")  # try something really weird
endfunction()
test_expect_safe()

function(test_expect_message)
	set(CMAKE_MESSAGE_LOG_LEVEL ERROR)  # comment-out to check error output for this function
	expect(FALSE MESSAGE "Assertion failed!" SAFE)
endfunction()
test_expect_message()

function(test_required)
	expect(TRUE REQUIRED)
	#expect(FALSE MESSAGE "Comment-out this test!" REQUIRED)  # uncomment to check error output
endfunction()
test_required()
