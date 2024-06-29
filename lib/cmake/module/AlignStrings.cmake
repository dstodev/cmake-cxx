include_guard()

include(SortListByLength)

#[[
	Given two strings string1 and string2, and a character to align on, prepends
	spaces to string1 or string2 until they align on the first occurrence of the
	align character.
]]
function(align_strings string1 string2 align)
	string(FIND "${${string1}}" "${align}" index1)
	string(FIND "${${string2}}" "${align}" index2)

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

	set(${string1} "${spaces1}${${string1}}" PARENT_SCOPE)
	set(${string2} "${spaces2}${${string2}}" PARENT_SCOPE)
endfunction()

function(align_list list_name align)
	sort_list_by_length(${list_name} sorted ORDER DESCENDING)
	list(FILTER sorted INCLUDE REGEX "${align}")
	list(LENGTH sorted length)

	if(length EQUAL 0)
		return()
	endif()

	list(GET sorted 0 largest)

	foreach(item IN LISTS ${list_name})
		align_strings(largest item ${align})
		list(APPEND aligned "${item}")
	endforeach()

	set(${list_name} "${aligned}" PARENT_SCOPE)
endfunction()

include(Expect)
expect_test_preamble()

function(test_align_strings_equal)
	set(str1 "Hello!")
	set(str2 "World!")

	align_strings(str1 str2 "!")

	expect(str1 MATCHES "^Hello!$")
	expect(str2 MATCHES "^World!$")
endfunction()
test_align_strings_equal()

function(test_align_strings_str1_longer)
	set(str1 "Hello, !")
	set(str2 "World!")

	align_strings(str1 str2 "!")

	expect(str1 MATCHES "^Hello, !$")
	expect(str2 MATCHES "^  World!$")
endfunction()
test_align_strings_str1_longer()

function(test_align_strings_str2_longer)
	set(str1 "Hello!")
	set(str2 ", World!")

	align_strings(str1 str2 "!")

	expect(str1 MATCHES "^  Hello!$")
	expect(str2 MATCHES "^, World!$")
endfunction()
test_align_strings_str2_longer()

function(test_align_strings_str1_empty)
	set(str1 "")
	set(str2 "World!")

	align_strings(str1 str2 "!")

	expect(str1 MATCHES "^$")
	expect(str2 MATCHES "^World!$")
endfunction()
test_align_strings_str1_empty()

function(test_align_strings_str2_empty)
	set(str1 "Hello!")
	set(str2 "")

	align_strings(str1 str2 "!")

	expect(str1 MATCHES "^Hello!$")
	expect(str2 MATCHES "^$")
endfunction()
test_align_strings_str2_empty()

function(test_align_list_no_align)
	set(list "Hello;, World")
	align_list(list "!")
	expect("${list}" STREQUAL "Hello;, World")
endfunction()
test_align_list_no_align()

function(test_align_list_one_align)
	set(list "Hello;, World!")
	align_list(list "!")
	expect("${list}" STREQUAL "Hello;, World!")
endfunction()
test_align_list_one_align()

set(check_output FALSE)

function(test_align_list_two_align)
	set(list "Hello, ;World!;Never mind, ;goodbye!")
	align_list(list "!")
	expect("${list}" STREQUAL "Hello, ;  World!;Never mind, ;goodbye!")
	if(check_output)
		list(JOIN list "\n" list)
		message("${list}")
	endif()
endfunction()
test_align_list_two_align()

function(test_align_list_all_align)
	set(list "Hello, !;World!;Never mind, !    ;goodbye!")
	align_list(list "!")
	expect("${list}" STREQUAL "     Hello, !;       World!;Never mind, !    ;     goodbye!")
	if(check_output)
		list(JOIN list "\n" list)
		message("${list}")
	endif()
endfunction()
test_align_list_all_align()

unset(check_output)
