#[[
	Given two strings str1 and str2, and a character to align on, prepends
	spaces to str1 or str2 until they align on the first occurrence of the
	character.
]]
macro(align_strings str1 str2 align)
	string(FIND "${${str1}}" "${align}" index1)
	string(FIND "${${str2}}" "${align}" index2)

	if(index1 EQUAL -1 OR index2 EQUAL -1)
		return()
	endif()

	math(EXPR index "${index1} - ${index2}")

	if(index GREATER 0)
		string(REPEAT " " ${index} spaces2)
	endif()

	math(EXPR index "${index2} - ${index1}")

	if(index GREATER 0)
		string(REPEAT " " ${index} spaces1)
	endif()

	set(${str1} "${spaces1}${${str1}}")
	set(${str2} "${spaces2}${${str2}}")
endmacro()

expect_test_preamble()

function(test_align_strings_equal)
	set(str1 "Hello!")
	set(str2 "World!")

	align_strings(str1 str2 "!")

	expect("${str1}" MATCHES "^Hello!$")
	expect("${str2}" MATCHES "^World!$")
endfunction()
test_align_strings_equal()

function(test_align_strings_str1_longer)
	set(str1 "Hello, !")
	set(str2 "World!")

	align_strings(str1 str2 "!")

	expect("${str1}" MATCHES "^Hello, !$")
	expect("${str2}" MATCHES "^  World!$")
endfunction()
test_align_strings_str1_longer()

function(test_align_strings_str2_longer)
	set(str1 "Hello!")
	set(str2 ", World!")

	align_strings(str1 str2 "!")

	expect("${str1}" MATCHES "^  Hello!$")
	expect("${str2}" MATCHES "^, World!$")
endfunction()
test_align_strings_str2_longer()

function(test_align_strings_str1_empty)
	set(str1 "")
	set(str2 "World!")

	align_strings(str1 str2 "!")

	expect("${str1}" MATCHES "^$")
	expect("${str2}" MATCHES "^World!$")
endfunction()
test_align_strings_str1_empty()

function(test_align_strings_str2_empty)
	set(str1 "Hello!")
	set(str2 "")

	align_strings(str1 str2 "!")

	expect("${str1}" MATCHES "^Hello!$")
	expect("${str2}" MATCHES "^$")
endfunction()
